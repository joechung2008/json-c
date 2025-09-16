#include <stdlib.h>
#include <string.h>
#include <json_c/json.h>
#include "json_internal.h"
#include "parsers/value.h"
#include "types/token.h"
#include "token_free.h"
#include "token_helpers.h"

/* Internal wrapper that can either store raw text (legacy) or a parsed Token* */
struct json_value
{
    char  *text;
    Token *token; /* owned token when set; otherwise NULL */
};

// legacy single-field struct removed (definition now above)

json_value_t *json_parse(const char *text)
{
    if (!text)
        return NULL;
    json_value_t *v = malloc(sizeof(*v));
    if (!v)
        return NULL;
    v->text = json_strdup(text);
    if (!v->text)
    {
        free(v);
        return NULL;
    }
    v->token = NULL;

    /* Try to parse into internal token tree so tests can introspect. If
     * parsing fails, free the allocated json_value_t and return NULL to
     * signal a parse error and avoid leaking the duplicated input.
     */
    char *copy = json_strdup(text);
    if (!copy)
    {
        /* strdup failed; cleanup and return error */
        free(v->text);
        free(v);
        return NULL;
    }

    Token *t = token_parse(copy, false);
    free(copy);
    if (!t)
    {
        /* Parsing failed. Free the textual copy and the json_value_t and
         * return NULL so callers see a parse failure (tests expect NULL
         * for malformed input).
         */
        free(v->text);
        free(v);
        return NULL;
    }

    v->token = t;
    return v;
}

void json_free(json_value_t *v)
{
    if (!v)
        return;
    if (v->text)
        free(v->text);
    if (v->token)
    {
        token_free(v->token);
    }
    free(v);
}

const char *json_text(const json_value_t *v)
{
    return v ? v->text : NULL;
}

int json_value_type(const json_value_t *v)
{
    if (!v || !v->token)
        return -1;
    /* token structs have 'type' field as second int (after skip) */
    int type = TOKEN_TYPE(v->token);
    switch (type)
    {
    case TOKEN_NULL:
        return JSON_NULL;
    case TOKEN_TRUE:
        return JSON_TRUE;
    case TOKEN_FALSE:
        return JSON_FALSE;
    case TOKEN_NUMBER:
        return JSON_NUMBER;
    case TOKEN_STRING:
        return JSON_STRING;
    case TOKEN_ARRAY:
        return JSON_ARRAY;
    case TOKEN_OBJECT:
        return JSON_OBJECT;
    case TOKEN_PAIR:
        return JSON_PAIR;
    default:
        return -1;
    }
}

int json_value_get_number(const json_value_t *v, double *out)
{
    if (!v || !v->token)
        return 0;
    int type = TOKEN_TYPE(v->token);
    if (type != TOKEN_NUMBER)
        return 0;
    NumberToken *n = (NumberToken *)v->token;
    if (out)
        *out = n->value;
    return 1;
}

const char *json_value_get_string(const json_value_t *v)
{
    if (!v || !v->token)
        return NULL;
    int type = TOKEN_TYPE(v->token);
    if (type != TOKEN_STRING)
        return NULL;
    StringToken *s = (StringToken *)v->token;
    return s->value;
}

int json_value_get_bool(const json_value_t *v, int *out)
{
    if (!v || !v->token)
        return 0;
    int type = TOKEN_TYPE(v->token);
    if (type == TOKEN_TRUE)
    {
        if (out)
            *out = 1;
        return 1;
    }
    if (type == TOKEN_FALSE)
    {
        if (out)
            *out = 0;
        return 1;
    }
    return 0;
}

size_t json_array_length(const json_value_t *v)
{
    if (!v || !v->token)
        return 0;
    int type = TOKEN_TYPE(v->token);
    if (type != TOKEN_ARRAY)
        return 0;
    ArrayToken *a = (ArrayToken *)v->token;
    if (!a->elements)
        return 0;
    return (size_t)a->elements->size;
}

json_value_t *json_array_get(const json_value_t *v, size_t idx)
{
    if (!v || !v->token)
        return NULL;
    int type = *((int *)(((char *)v->token) + sizeof(int)));
    if (type != TOKEN_ARRAY)
        return NULL;
    ArrayToken *a = (ArrayToken *)v->token;
    if (!a->elements || idx >= (size_t)a->elements->size)
        return NULL;
    Token *child = a->elements->tokens[idx];
    /* Wrap child token in a json_value_t for convenience (non-owning)
     * We'll create a json_value_t that points to the child's memory but does
     * not take ownership (so freeing it won't free tokens). The owner is
     * the parent v. To keep API simple, return a wrapper with token set but
     * text == NULL and token == child; caller must not call json_free on it.
     */
    json_value_t *wrap = malloc(sizeof(*wrap));
    if (!wrap)
        return NULL;
    wrap->text  = NULL;
    wrap->token = child;
    return wrap;
}

size_t json_object_size(const json_value_t *v)
{
    if (!v || !v->token)
        return 0;
    int type = TOKEN_TYPE(v->token);
    if (type != TOKEN_OBJECT)
        return 0;
    ObjectToken *o = (ObjectToken *)v->token;
    if (!o->members)
        return 0;
    return (size_t)o->members->size;
}

const char *json_object_key(const json_value_t *v, size_t idx)
{
    if (!v || !v->token)
        return NULL;
    int type = TOKEN_TYPE(v->token);
    if (type != TOKEN_OBJECT)
        return NULL;
    ObjectToken *o = (ObjectToken *)v->token;
    if (!o->members || idx >= (size_t)o->members->size)
        return NULL;
    PairToken *p = o->members->pairs[idx];
    if (!p || !p->key)
        return NULL;
    return p->key->value;
}

json_value_t *json_object_value(const json_value_t *v, size_t idx)
{
    if (!v || !v->token)
        return NULL;
    int type = TOKEN_TYPE(v->token);
    if (type != TOKEN_OBJECT)
        return NULL;
    ObjectToken *o = (ObjectToken *)v->token;
    if (!o->members || idx >= (size_t)o->members->size)
        return NULL;
    PairToken *p = o->members->pairs[idx];
    if (!p || !p->value)
        return NULL;
    json_value_t *wrap = malloc(sizeof(*wrap));
    if (!wrap)
        return NULL;
    wrap->text  = NULL;
    wrap->token = p->value;
    return wrap;
}

void json_value_free_wrapper(json_value_t *v)
{
    if (!v)
        return;
    /* Only free the wrapper struct and any non-owned text pointer (none set) */
    free(v);
}
