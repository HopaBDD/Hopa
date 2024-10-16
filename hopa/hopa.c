#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define STRINGIFY(_str)             #_str
#define TOSTRING(_str)              STRINGIFY(_str)

#define FUNC_NAME_HELPER_Y(_line)   hopa_func_##_line
#define FUNC_NAME_HELPER_X(_line)   FUNC_NAME_HELPER_Y(_line)
#define FUNC_NAME                   FUNC_NAME_HELPER_X(__LINE__)

#define RUNIT_NESTING_COUNT         10
#define RUNIT_DESC_LEN              (100 + 1)
#define RUNIT_CHECK_FIRST_FAILURE   hfw_s.cnt_failure != 1 ? true : printf("\nFailures:\n\n")
#define RUNIT_ERROR_TEXT            printf("%d) %s\n\t\033[31mError %s(%d): \033[0m", hfw_s.cnt_failure, hfw_s.err_buf, __FILE__, __LINE__)
#define RUNIT_IF_EXPECT(_x,_y)      hfw_s.is_expect == true ? hfw_s.is_expect = false, printf("\tExpected: %s\n\tGot: %u\n\n\t(compared using %s)\n\n", _x, hfw_s.expect_val, _y) : true

#define eq_expr_template(_sign, _text, _x) \
                                    _sign _x ? \
                                    true : \
                                    (    hfw_s.cnt_failure++, \
                                         RUNIT_CHECK_FIRST_FAILURE, \
                                         RUNIT_ERROR_TEXT, \
                                         printf("%s %s (%d dec/0x%x hex)\n\n", _text, #_x, _x, _x), \
                                         RUNIT_IF_EXPECT(#_x, #_sign) \
                                    ); \
                                    hfw_s.test_num++;

#define to_eq(_x)                   eq_expr_template(==, "Expected to equal to", _x)
#define not_to_eq(_x)               eq_expr_template(!=, "Expected to not to equal to", _x)
#define less_then(_x)               eq_expr_template(< , "Expected to be less then", _x)
#define greater_then(_x)            eq_expr_template(> , "Expected to be greater then", _x)

#define arr_to_eq(_a, _b, _s)       for (int i = 0; i < _s; i++) \
                                    { \
                                        if (_a[i] != _b[i]) \
                                        { \
                                            hfw_s.cnt_failure++; \
                                            RUNIT_CHECK_FIRST_FAILURE; \
                                            RUNIT_ERROR_TEXT; \
                                            printf("Elemets with %d index not equal (0x%x != 0x%x)\n\n", i, _a[i], _b[i]); \
                                        } \
                                    } \
                                    hfw_s.test_num++;

#define assert(_text)               (   hfw_s.cnt_failure++, \
                                        RUNIT_CHECK_FIRST_FAILURE, \
                                        RUNIT_ERROR_TEXT, \
                                        printf("[Assert] %s\n\n", _text) \
                                    ); \
                                    hfw_s.test_num++;

#define assert_failure()            assert("Failure stub")

#define describe(_str)              snprintf(hfw_s.tmp_str[hfw_s.namespace], RUNIT_DESC_LEN, "%s", _str); \
                                    auto void FUNC_NAME(void); \
                                    snprintf(hfw_s.err_buf, RUNIT_DESC_LEN, "%s", hfw_s.tmp_str[0]); \
                                    for (int i = 1; i <= hfw_s.namespace; i++) { strcat(hfw_s.err_buf, "\n\t"); strcat(hfw_s.err_buf, hfw_s.tmp_str[i]); } \
                                    if (ru_before_each_func != NULL && hfw_s.is_it) { ru_before_each_func(); } \
                                    hfw_s.namespace++; \
                                    if (hfw_s.namespace > RUNIT_NESTING_COUNT) { printf("\n[hopa] Error: The nesting of blocks should not exceed the maximum (%d)\n\n", RUNIT_NESTING_COUNT); exit(1); } \
                                    FUNC_NAME(); \
                                    hfw_s.namespace--; \
                                    hfw_s.is_it = false; \
                                    void FUNC_NAME(void)

#define context(_str)               describe(_str)

#define it(_str)                    hfw_s.is_it = true; \
                                    describe(_str) 

#define expect(_s)                  hfw_s.is_expect = true; hfw_s.expect_val = _s; _s

#define source(_str)

#define before_each  void ru_before_each_func()

__attribute__((weak)) void ru_before_each_func();

int main(void)
{
    FILE *fptr;
    int   ret;

    struct hopa_fw_struct_s
    {
        unsigned int    test_num;
        unsigned int    cnt_failure;
        char            tmp_str[RUNIT_NESTING_COUNT][RUNIT_DESC_LEN];
        char            err_buf[RUNIT_NESTING_COUNT * RUNIT_DESC_LEN + RUNIT_NESTING_COUNT * 2];
        unsigned int    namespace;
        bool            is_it;
        unsigned int    expect_val;
        bool            is_expect;
    };

    struct hopa_fw_struct_s hfw_s = 
    {
        .test_num       = 0,
        .cnt_failure    = 0,
        .tmp_str        = { 0 },
        .err_buf        = "",
        .namespace      = 0,
        .is_it          = false,
        .is_expect      = false
    };

    fptr = fopen(TOSTRING(HOPA_RES_FILE), "r");
    if (fptr == NULL)
    {
        printf("[Hopa] Error: Cannot open file %s for read\n", TOSTRING(HOPA_RES_FILE));
        return 1;
    }

    ret = fscanf(fptr, "%u%u", &hfw_s.test_num, &hfw_s.cnt_failure);
    if (ret != 2)
    {
        printf("[Hopa] Error: The content of file %s does not match\n", TOSTRING(HOPA_RES_FILE));
        return 1;
    }

    #include "includes"

    fptr = freopen(TOSTRING(HOPA_RES_FILE), "w", fptr);
    if (fptr == NULL)
    {
        printf("[Hopa] Error: Cannot open file %s\n", TOSTRING(HOPA_RES_FILE));
        return 1;
    }

    ret = fprintf(fptr, "%u %u\n", hfw_s.test_num, hfw_s.cnt_failure);
    fclose(fptr);
    if (ret < 4)
    {
        printf("[Hopa] Error: Cannot write to the %s file\n", TOSTRING(HOPA_RES_FILE));
        return 1;
    }

    return 0;
}
