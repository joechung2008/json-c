#ifndef JSON_C_TYPES_H
#define JSON_C_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* Public, portable typedefs for json-c */
    typedef int32_t  json_int32_t;
    typedef uint32_t json_uint32_t;
    typedef int64_t  json_int64_t;
    typedef uint64_t json_uint64_t;

#ifdef __cplusplus
}
#endif

#endif /* JSON_C_TYPES_H */
