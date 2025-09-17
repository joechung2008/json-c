#include "../json_internal.h"
#include "./shared.h"

// used by number.c and string.c
char *strncat_dynamic(char *dst, const char *src, size_t n)
{
    if (!src)
        return dst;

    size_t src_len = json_strnlen(src, n);
    if (dst == NULL)
    {
        dst = (char *)calloc(src_len + 1, sizeof(char));
        if (!dst)
            return NULL;
        if (json_memcpy(dst, src_len + 1, src, src_len) != 0)
        {
            free(dst);
            return NULL;
        }
        dst[src_len] = '\0';
        return dst;
    }

    size_t dst_len = strlen(dst);
    /* Use a temporary pointer for realloc to avoid losing the original
     * buffer if allocation fails. Do not free the original here; leave
     * error handling to the caller to be consistent with other paths.
     */
    char *tmp = (char *)realloc(dst, dst_len + src_len + 1);
    if (!tmp)
    {
        return NULL;
    }
    if (json_memcpy(tmp + dst_len, src_len + 1, src, src_len) != 0)
    {
        return NULL;
    }
    tmp[dst_len + src_len] = '\0';
    return tmp;
}
