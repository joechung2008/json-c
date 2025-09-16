#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json_c/json.h>

/* Read entire stdin into a heap buffer. Caller must free returned pointer. */
static char *read_stdin_all(size_t *out_len)
{
    size_t size = 0;
    size_t cap  = 4096;
    char  *buf  = malloc(cap);
    if (!buf)
        return NULL;

    for (;;)
    {
        size_t n = fread(buf + size, 1, cap - size, stdin);
        size += n;
        if (size < cap)
            break; /* EOF or short read */

        /* need more space */
        size_t newcap = cap * 2;
        char  *nb     = realloc(buf, newcap);
        if (!nb)
        {
            free(buf);
            return NULL;
        }
        buf = nb;
        cap = newcap;
    }

    /* NUL-terminate */
    if (size + 1 > cap)
    {
        char *nb = realloc(buf, size + 1);
        if (!nb)
        {
            free(buf);
            return NULL;
        }
        buf = nb;
    }
    buf[size] = '\0';
    if (out_len)
        *out_len = size;
    return buf;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    size_t len   = 0;
    char  *input = read_stdin_all(&len);
    if (!input)
    {
        fprintf(stderr, "failed to read stdin\n");
        return 2;
    }

    /* If no data was provided, treat as error */
    if (len == 0)
    {
        fprintf(stderr, "no input provided\n");
        free(input);
        return 3;
    }

    json_value_t *v = json_parse(input);
    if (!v)
    {
        fprintf(stderr, "parse failed: %s\n", input);
        free(input);
        return 1;
    }

    /* json_parse may return a json_value_t that only contains the original
     * text when token parsing failed; in that case treat it as a parse
     * failure for CLI consumers.
     */
    if (json_value_type(v) == -1)
    {
        fprintf(stderr, "parse failed: %s\n", input);
        json_free(v);
        free(input);
        return 1;
    }

    const char *out = json_text(v);
    if (out)
        fputs(out, stdout);
    else
        fprintf(stderr, "json_text returned NULL\n");

    json_free(v);
    free(input);
    return 0;
}
