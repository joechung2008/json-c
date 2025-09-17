#include "../json_internal.h"
#include "./string.h"
#include "./shared.h"

// BUG \uxxxx fails when xxxx > 0xFF
StringToken *json_parse_string(const char *s)
{
    enum
    {
        Scanning,
        Char,
        EscapedChar,
        Unicode,
        End
    };

    int   mode  = Scanning;
    int   pos   = 0;
    char *value = NULL;

    while (pos < strlen(s) && mode != End)
    {
        char ch = s[pos];

        switch (mode)
        {
        case Scanning:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            { // [ \n\r\t]
                pos++;
            }
            else if (ch == '"')
            {
                value = strncat_dynamic(NULL, "", 0);
                pos++;
                mode = Char;
            }
            else
            {
                goto fail;
            }
            break;

        case Char:
            if (ch == '\\')
            {
                pos++;
                mode = EscapedChar;
            }
            else if (ch == '"')
            {
                pos++;
                mode = End;
            }
            else if (ch != '\n' && ch != '\r')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
            }
            else
            {
                goto fail;
            }
            break;

        case EscapedChar:
            if (ch == '"' || ch == '\\' || ch == '/')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
                mode = Char;
            }
            else if (ch == 'b')
            {
                value = strncat_dynamic(value, "\b", 1);
                pos++;
                mode = Char;
            }
            else if (ch == 'f')
            {
                value = strncat_dynamic(value, "\f", 1);
                pos++;
                mode = Char;
            }
            else if (ch == 'n')
            {
                value = strncat_dynamic(value, "\n", 1);
                pos++;
                mode = Char;
            }
            else if (ch == 'r')
            {
                value = strncat_dynamic(value, "\r", 1);
                pos++;
                mode = Char;
            }
            else if (ch == 't')
            {
                value = strncat_dynamic(value, "\t", 1);
                pos++;
                mode = Char;
            }
            else if (ch == 'u')
            {
                pos++;
                mode = Unicode;
            }
            else
            {
                goto fail;
            }
            break;

        case Unicode:
        {
            long code = parse_hexadecimal_code(s + pos);
            if (code != -1)
            {
                /*
                 * SAFE APPEND EXPLANATION:
                 * The original code attempted to do:
                 *     value = strncat_dynamic(value, &((char)code), 1);
                 * That is invalid because (char)code is an rvalue (a temporary)
                 * and you cannot take the address of an rvalue in C. Even if
                 * a compiler allowed it, passing the address of a non-NUL
                 * terminated temporary into strncat would be unsafe.
                 *
                 * Minimal safe fix: create a small NUL-terminated buffer
                 * on the stack containing the single character, and pass
                 * that buffer to the concatenation helper.
                 */
                char chbuf[2] = {(char)code, '\0'};
                value         = strncat_dynamic(value, chbuf, 1);
                if (!value)
                    goto fail;
                pos += 4;
                mode = Char;
            }
            else
            {
                goto fail;
            }
        }
        break;

        case End:
            break;

        default:
            goto fail;
        }
    }

    /* Only accept a string token if we reached the End state (closing quote). */
    if (mode != End)
        goto fail;

    StringToken *stringtokenp = (StringToken *)malloc(sizeof(StringToken));
    if (!stringtokenp)
    {
        goto fail;
    }
    stringtokenp->skip  = pos;
    stringtokenp->type  = TOKEN_STRING;
    stringtokenp->value = value;
    return stringtokenp;

fail:
    if (value)
        free(value);
    return NULL;
}

static long parse_hexadecimal_code(const char *s)
{
    /*
     * Use a writable local buffer for the hex string. Do NOT write into
     * a string literal (which may be in read-only memory). We expect four
     * hex digits starting at `s`.
     */
    char hex[7] = "0x0000"; /* 6 chars + NUL */
    /* Ensure the input has at least 4 characters to copy. */
    if (json_strnlen(s, 5) < 4)
        return -1l;
    _Static_assert(sizeof(hex) >= 7, "hex buffer too small");
    if (json_memcpy(hex + 2, sizeof hex - 2, s, 4) != 0)
        return -1l;
    hex[6] = '\0';

    char *endptr;
    long  code = strtol(hex, &endptr, 0);
    /* Accept only if exactly the 4 hex digits were consumed (endptr points at the NUL) */
    if (endptr == hex + 6)
    {
        return code;
    }

    return -1l;
}
