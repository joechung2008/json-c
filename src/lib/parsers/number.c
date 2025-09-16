#include <stdio.h>

#include "number.h"
#include "shared.h"

NumberToken *json_parse_number(char *s, char *delimiters)
{
    enum
    {
        Scanning,
        Characteristic,
        CharacteristicDigit,
        DecimalPoint,
        Mantissa,
        Exponent,
        ExponentSign,
        ExponentFirstDigit,
        ExponentDigits,
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
            else if (ch == '-')
            {
                value = strncat_dynamic(value, "-", 1);
                pos++;
            }
            mode = Characteristic;
            break;

        case Characteristic:
            if (ch == '0')
            {
                value = strncat_dynamic(value, "0", 1);
                pos++;
                mode = DecimalPoint;
            }
            else if (ch >= '1' && ch <= '9')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
                mode = CharacteristicDigit;
            }
            else
            {
                goto fail;
            }
            break;

        case CharacteristicDigit:
            if (ch >= '0' && ch <= '9')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
            }
            else if (delimiters != NULL && strchr(delimiters, ch) != NULL)
            {
                mode = End;
            }
            else
            {
                mode = DecimalPoint;
            }
            break;

        case DecimalPoint:
            if (ch == '.')
            {
                value = strncat_dynamic(value, ".", 1);
                pos++;
                mode = Mantissa;
            }
            else if (delimiters != NULL && strchr(delimiters, ch) != NULL)
            {
                mode = End;
            }
            else
            {
                mode = Exponent;
            }
            break;

        case Mantissa:
            if (ch >= '0' && ch <= '9')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
            }
            else if (ch == 'e' || ch == 'E')
            {
                mode = Exponent;
            }
            else if (delimiters != NULL && strchr(delimiters, ch) != NULL)
            {
                mode = End;
            }
            else
            {
                goto fail;
            }
            break;

        case Exponent:
            if (ch == 'e' || ch == 'E')
            {
                value = strncat_dynamic(value, "e", 1);
                pos++;
                mode = ExponentSign;
            }
            else
            {
                goto fail;
            }
            break;

        case ExponentSign:
            if (ch == '+' || ch == '-')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
            }
            mode = ExponentFirstDigit;
            break;

        case ExponentFirstDigit:
            if (ch >= '0' && ch <= '9')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
                mode = ExponentDigits;
            }
            else
            {
                goto fail;
            }
            break;

        case ExponentDigits:
            if (ch >= '0' && ch <= '9')
            {
                value = strncat_dynamic(value, &ch, 1);
                pos++;
            }
            else if (delimiters != NULL && strchr(delimiters, ch) != NULL)
            {
                mode = End;
            }
            else
            {
                goto fail;
            }
            break;

        case End:
            break;

        default:
            goto fail;
        }
    }

    double number;
    /* sscanf returns the number of items successfully parsed; require 1 */
    if (!value || sscanf(value, "%lf", &number) != 1)
        goto fail;

    NumberToken *numbertokenp = (NumberToken *)malloc(sizeof(NumberToken));
    if (!numbertokenp)
        goto fail;

    numbertokenp->skip  = pos;
    numbertokenp->type  = TOKEN_NUMBER;
    numbertokenp->value = number;
    if (value)
        free(value);
    return numbertokenp;

fail:
    if (value)
        free(value);
    return NULL;
}
