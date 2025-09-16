#ifndef JSON_C_JSON_H
#define JSON_C_JSON_H

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct json_value json_value_t;

    /* Parse JSON text and return a new json_value_t*, or NULL on error */
    json_value_t *json_parse(const char *text);

    /* Free a json_value_t created by json_parse. If the value is a wrapper
     * around an internal token (created by accessors), the token is not freed.
     */
    void json_free(json_value_t *v);

    /* Return a pointer to an internal string representation (read-only). */
    const char *json_text(const json_value_t *v);

    /* Introspection API: expose parsed token types and values so tests can
     * inspect the parsed structure without relying on json_text parsing.
     */
    typedef enum
    {
        JSON_NULL = 0,
        JSON_TRUE,
        JSON_FALSE,
        JSON_NUMBER,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT,
        JSON_PAIR
    } json_type_t;

    /* Get the type of the parsed value. Returns one of json_type_t or -1 for error. */
    int json_value_type(const json_value_t *v);

    /* Get numeric value. Returns 1 on success and writes the value to out.
     * Returns 0 if the value is not numeric.
     */
    int json_value_get_number(const json_value_t *v, double *out);

    /* Get string value. Returns pointer to NUL-terminated string owned by the
     * token (valid until the root json_value is freed), or NULL if not a string.
     */
    const char *json_value_get_string(const json_value_t *v);

    /* Boolean getter. Returns 1 on success and writes 0/1 to out; returns 0 if
     * not a boolean value.
     */
    int json_value_get_bool(const json_value_t *v, int *out);

    /* Array/object accessors */
    size_t        json_array_length(const json_value_t *v);
    json_value_t *json_array_get(const json_value_t *v, size_t idx);

    size_t        json_object_size(const json_value_t *v);
    const char   *json_object_key(const json_value_t *v, size_t idx);
    json_value_t *json_object_value(const json_value_t *v, size_t idx);

    /* Free a wrapper returned by array/object getters. This will free the small
     * wrapper object but will not free the underlying token owned by the root
     * json_value_t.
     */
    void json_value_free_wrapper(json_value_t *v);

#ifdef __cplusplus
}
#endif

#endif /* JSON_C_JSON_H */
