#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* Internal-only helpers for json-c */

    /* Duplicate a string in a portable way. Returns heap-allocated copy or NULL. */
    char *json_strdup(const char *s);

    /* Portable checked memcpy: returns 0 on success, -1 on error. dst_size is
     * the size in bytes of the destination buffer. Implemented in
     * json_compat.c as json_memcpy().
     */
    int json_memcpy(void *restrict dst, size_t dst_size, const void *restrict src, size_t n);

    /* Locale-invariant string-to-double parser.
     * Parses a C-style floating point number using '.' as the decimal separator
     * regardless of the current LC_NUMERIC. The function mirrors strtod's
     * behavior: returns the converted value and stores the address of the
     * first unconverted character in endptr (if non-NULL).
     */
    double json_strtod(const char *nptr, char **endptr);

#ifdef __cplusplus
}
#endif
