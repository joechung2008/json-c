/* Portable duplication helper for internal use */
#include "json_internal.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
/* C23 added strdup but not a locale-specific strtod wrapper; feature-test below */
#endif

#if defined(__unix__) || defined(__APPLE__)
#define HAVE_STRTOD_L 1
#include <xlocale.h>
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
    memcpy(p, s, n);
    return p;
#endif
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
    /* POSIX-ish platforms: use strtod_l with newlocale/uselocale if available */
#if defined(__GLIBC__) || defined(__APPLE__)
    locale_t loc = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
    if (loc)
    {
        double val = strtod_l(nptr, endptr, loc);
        freelocale(loc);
        return val;
    }
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
