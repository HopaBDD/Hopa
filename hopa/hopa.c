#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define FUNC_NAME_HELPER_Y(_line)   hopa_func_##_line
#define FUNC_NAME_HELPER_X(_line)   FUNC_NAME_HELPER_Y(_line)
#define FUNC_NAME                   FUNC_NAME_HELPER_X(__LINE__)

#define RUNIT_NESTING_COUNT         10
#define RUNIT_DESC_LEN              (100 + 1)
#define RUNIT_CHECK_FIRST_FAILURE   ru_count_failure != 1 ? true : printf("\nFailures:\n\n")
#define RUNIT_ERROR_TEXT            printf("%d) %s\n\tError %s(%d): ", ru_count_failure, ru_err_buf, __FILE__, __LINE__)
#define RUNIT_IF_EXPECT(_x,_y)      ru_is_expect == true ? ru_is_expect = false, printf("\n\tExpected: %s\n\tGot: %u\n\n\t(compared using %s)\n\n", _x, ru_expect_val, _y) : true

#define to_eq(_x)                    == _x ? \
                                    true : \
                                    (    ru_count_failure++, \
                                         RUNIT_CHECK_FIRST_FAILURE, \
                                         RUNIT_ERROR_TEXT, \
                                         printf("Expected to equal to %s (%d dec/0x%x hex)\n", #_x, _x, _x), \
                                         RUNIT_IF_EXPECT(#_x, "==") \
                                    ); \
                                    ru_test++;

#define not_to_eq(_x)                != _x ? \
                                    true : \
                                    (   ru_count_failure++, \
                                        RUNIT_CHECK_FIRST_FAILURE, \
                                        RUNIT_ERROR_TEXT, \
                                        printf("Expected to not to equal to %s (%d dec/0x%x hex)\n", #_x, _x, _x), \
                                        RUNIT_IF_EXPECT(#_x, "!=") \
                                    ); \
                                    ru_test++;

#define arr_to_eq(_a, _b, _s)       for (int i = 0; i < _s; i++) \
                                    { \
                                        if (_a[i] != _b[i]) \
                                        { \
                                            ru_count_failure++; \
                                            RUNIT_CHECK_FIRST_FAILURE; \
                                            RUNIT_ERROR_TEXT; \
                                            printf("Elemets with %d index not equal (0x%x != 0x%x)\n", i, _a[i], _b[i]); \
                                        } \
                                    } \
                                    ru_test++;

#define assert_false()              (   ru_count_failure++, \
                                        RUNIT_CHECK_FIRST_FAILURE, \
                                        RUNIT_ERROR_TEXT, \
                                        printf("Failure stub\n\n") \
                                    ); \
                                    ru_test++;

#define describe(_str)              snprintf(ru_tmp_str[ru_namespace], RUNIT_DESC_LEN, "%s", _str); \
                                    auto void FUNC_NAME(void); \
                                    snprintf(ru_err_buf, RUNIT_DESC_LEN, "%s", ru_tmp_str[0]); \
                                    for (int i = 1; i <= ru_namespace; i++) { strcat(ru_err_buf, "\n\t"); strcat(ru_err_buf, ru_tmp_str[i]); } \
                                    if (ru_before_each_func != NULL && ru_is_it) { ru_before_each_func(); } \
                                    ru_namespace++; \
                                    if (ru_namespace > RUNIT_NESTING_COUNT) { printf("\n[hopa] Error: The nesting of blocks should not exceed the maximum (%d)\n\n", RUNIT_NESTING_COUNT); exit(1); } \
                                    FUNC_NAME(); \
                                    ru_namespace--; \
                                    ru_is_it = 0; \
                                    void FUNC_NAME(void)

#define context(_str)               describe(_str)

#define it(_str)                    ru_is_it = 1; \
                                    describe(_str) 

#define expect(_s)                  ru_is_expect = true; ru_expect_val = _s; _s

#define source(_str)

#define before_each  void ru_before_each_func()

__attribute__((weak)) void ru_before_each_func();

unsigned int    ru_test = 0;
unsigned int    ru_count_failure = 0;
char            ru_tmp_str[RUNIT_NESTING_COUNT][RUNIT_DESC_LEN];
char            ru_err_buf[RUNIT_NESTING_COUNT * RUNIT_DESC_LEN + RUNIT_NESTING_COUNT * 2] = "";
unsigned int    ru_namespace = 0;
char            ru_is_it = 0;
unsigned int    ru_expect_val;
bool            ru_is_expect = false;

int main(void)
{
    #include "includes"
    printf("\n%d examples, %d failures\n", ru_test, ru_count_failure);
    return 0;
}
