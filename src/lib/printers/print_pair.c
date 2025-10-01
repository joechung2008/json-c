#include <stdio.h>
#include "../types/pairtoken.h"
#include "../json_internal.h"

int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz)
{
    int written = json_snprintf(out, outsz, "%*sPairToken { skip: %d,\n", indent, "", tok->skip);
    int total   = written > 0 ? written : 0;
    if (tok->key)
    {
        extern int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz);
        written = json_snprintf(out + total, outsz - total, "%*skey: ", indent + 2, "");
        total += (written > 0 ? written : 0);
        written = print_string_token(tok->key, 0, out + total, outsz - total);
        total += (written > 0 ? written : 0);
    }
    if (tok->value)
    {
        extern int print_token(const Token *tok, int indent, char *out, size_t outsz);
        int        value_type = ((const int32_t *)tok->value)[1];
        if (value_type == TOKEN_ARRAY || value_type == TOKEN_OBJECT)
        {
            // Print 'value:' label, then value immediately after, no extra spaces or newlines
            int n = snprintf(out + total, outsz - total, "%*svalue: ", indent + 2, "");
            total += (n > 0 ? n : 0);
            // Print value token with indent+2, but suppress leading indent/newline in value printer
            // by passing a flag (if needed) or by trimming in the value printer. For now, hack: print_token at
            // indent+2, then skip leading spaces/newline.
            int value_written = print_token(tok->value, indent + 2, out + total, outsz - total);
            // If the value printer starts with spaces or a newline, skip them
            if (value_written > 0 && (out + total)[0] == ' ')
            {
                // skip leading spaces
                int skip = 0;
                while (skip < value_written && (out + total)[skip] == ' ')
                    ++skip;
                memmove(out + total, out + total + skip, value_written - skip);
                value_written -= skip;
            }
            if (value_written > 0 && (out + total)[0] == '\n')
            {
                memmove(out + total, out + total + 1, value_written - 1);
                value_written -= 1;
            }
            total += (value_written > 0 ? value_written : 0);
        }
        else
        {
            written = json_snprintf(out + total, outsz - total, "%*svalue: ", indent + 2, "");
            total += (written > 0 ? written : 0);
            written = print_token(tok->value, 0, out + total, outsz - total);
            total += (written > 0 ? written : 0);
        }
    }
    if (total < (int)outsz)
        total += json_snprintf(out + total, outsz - total, "%*s}\n", indent, "");
    return total;
}
