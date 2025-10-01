#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

Test(json_parse, test_parse_simple)
{
    const char   *text = "{\"ok\":true}";
    json_value_t *v    = json_parse(text);
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_OBJECT);
    cr_assert_eq((int)json_object_size(v), 1);
    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "ok");
    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    cr_assert_eq(json_value_type(val), JSON_TRUE);
    bool b = false;
    cr_assert(json_value_get_bool(val, &b));
    cr_assert(b == true);
    json_value_free_wrapper(val);
    json_free(v);
}
