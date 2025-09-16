/* Portable duplication helper for internal use */
#include "json_internal.h"
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
/* C23 added strdup but not a locale-specific strtod wrapper; feature-test below */
#endif

#if defined(_MSC_VER)
#include <string.h> /* for memcpy_s */
#endif

/* json_memcpy: checked memcpy wrapper. Returns 0 on success, -1 on error. */
int json_memcpy(void *restrict dst, size_t dst_size, const void *restrict src, size_t n)
{
#if defined(_MSC_VER)
    if (memcpy_s(dst, dst_size, src, n) != 0)
        return -1;
    return 0;
#else
    if (dst == NULL || src == NULL)
        return -1;
    if (dst_size < n)
        return -1;
    memcpy(dst, src, n);
    return 0;
#endif
}

#if defined(__unix__) || defined(__APPLE__)
#define HAVE_STRTOD_L 1
/* Some platforms provide <xlocale.h> (older glibc); prefer including it
 * only when the compiler's __has_include reports it exists. On other
 * systems (including modern glibc), <locale.h> exposes newlocale/strtod_l
 * and we don't need <xlocale.h>. */
#if defined(__has_include)
#if __has_include(<xlocale.h>)
#include <xlocale.h>
#endif
#endif
#endif

#if defined(_MSC_VER)
#define HAVE__STRTOD_L 1
#include <locale.h>
#endif

char *json_strdup(const char *s)
{
    if (!s)
        return NULL;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
    return strdup(s);
#elif defined(_MSC_VER)
    return _strdup(s);
#else
    size_t n = strlen(s) + 1;
    char  *p = (char *)malloc(n);
    if (!p)
        return NULL;
    if (json_memcpy(p, n, s, n) != 0)
    {
        free(p);
        return NULL;
    }
    return p;
#endif
}

/* Portable json_strnlen implementation. Use memchr to avoid relying on
 * a possibly missing prototype for strnlen on some platforms/feature tests.
 */
size_t json_strnlen(const char *s, size_t maxlen)
{
    const void *p = memchr(s, '\0', maxlen);
    return p ? (size_t)((const char *)p - s) : maxlen;
}

/* Locale-invariant wrapper around strtod. Prefer locale-specific APIs when
 * available (strtod_l/_strtod_l). Otherwise temporarily switch LC_NUMERIC to
 * the "C" locale, call strtod, and restore the previous locale. Note: changing
 * the global locale with setlocale is not thread-safe; platforms that support
 * the _l variants avoid that.
 */
double json_strtod(const char *nptr, char **endptr)
{
#if defined(HAVE_STRTOD_L) && !defined(_MSC_VER)
    /* POSIX-ish platforms: prefer strtod_l/newlocale when available. We
     * need to guard use of locale_t/LC_NUMERIC_MASK which may not exist on
     * all platforms even if strtod_l is present. */
#if defined(__has_include)
#if __has_include(<locale.h>) && (__has_include(<xlocale.h>) || defined(__APPLE__))
    /* Platforms that expose newlocale/strtod_l (glibc via xlocale.h or
     * BSD/Apple) will compile the following. Use newlocale when present. */
    locale_t loc = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
    if (loc)
    {
        double val = strtod_l(nptr, endptr, loc);
        freelocale(loc);
        return val;
    }
#endif
#endif
    /* Fallback to global strtod if newlocale is not available */
    return strtod(nptr, endptr);
#elif defined(HAVE__STRTOD_L) && defined(_MSC_VER)
    /* MSVC provides _strtod_l and _create_locale */
    _locale_t loc = _create_locale(LC_NUMERIC, "C");
    if (loc)
    {
        double val = _strtod_l(nptr, endptr, loc);
        _free_locale(loc);
        return val;
    }
    return strtod(nptr, endptr);
#else
    /* Portable fallback: save, set LC_NUMERIC to "C", call strtod, restore. */
    char *saved = NULL;
    /* setlocale(NULL) returns a pointer into internal storage; duplicate it */
    const char *cur = setlocale(LC_NUMERIC, NULL);
    if (cur)
    {
        saved = json_strdup(cur);
    }
    errno = 0;
    setlocale(LC_NUMERIC, "C");
    double val = strtod(nptr, endptr);
    if (saved)
    {
        setlocale(LC_NUMERIC, saved);
        free(saved);
    }
    else
    {
        /* if we couldn't save previous locale, just set it to the default "" */
        setlocale(LC_NUMERIC, "");
    }
    return val;
#endif
}
