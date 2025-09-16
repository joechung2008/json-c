#include "shared.h"

// used by number.c and string.c
char *strncat_dynamic(char *dst, const char *src, size_t n)
{
    if (!src)
        return dst;

    size_t src_len = strnlen(src, n);
    if (dst == NULL)
    {
        dst = (char *)calloc(src_len + 1, sizeof(char));
        if (!dst)
            return NULL;
        memcpy(dst, src, src_len);
        dst[src_len] = '\0';
        return dst;
    }

    size_t dst_len = strlen(dst);
    char  *newp    = (char *)realloc(dst, dst_len + src_len + 1);
    if (!newp)
    {
        free(dst);
        return NULL;
    }
    memcpy(newp + dst_len, src, src_len);
    newp[dst_len + src_len] = '\0';
    return newp;
}
