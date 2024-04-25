#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define FUNK_NAME_HELPER_Y(_line)   hopa_func_##_line
#define FUNK_NAME_HELPER_X(_line)   FUNK_NAME_HELPER_Y(_line)
#define FUNK_NAME                   FUNK_NAME_HELPER_X(__LINE__)

#define RUNIT_ATTACH_COUNT          10
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

#define assert_false                (   ru_count_failure++, \
                                        RUNIT_CHECK_FIRST_FAILURE, \
                                        RUNIT_ERROR_TEXT, \
                                        printf("Failure stub\n") \
                                    ); \
                                    ru_test++;

#define describe(_str)              ru_namespace++; \
                                    ru_tmp_str[ru_namespace] = _str; \
                                    auto void FUNK_NAME(char* ru_str); \
                                    sprintf(ru_err_buf, "%s", ru_tmp_str[1]); \
                                    for (int i = 2; i <= ru_namespace; i++) { strcat(ru_err_buf, "\n\t"); strcat(ru_err_buf, ru_tmp_str[i]); } \
                                    if (ru_before_each_func != NULL && ru_is_it) ru_before_each_func(); \
                                    FUNK_NAME(ru_tmp_str[ru_namespace]); \
                                    sprintf(ru_err_buf, "%s", ""); \
                                    ru_namespace--; \
                                    if (ru_namespace == 0) { /*ru_before_each_func = (void*)NULL*/ } \
                                    ru_is_it = 0; \
                                    void FUNK_NAME(char* ru_str)

#define context(_str)               describe(_str)

#define it(_str)                    ru_is_it = 1; \
                                    describe(_str) 

#define expect(_s)                  ru_is_expect = true; ru_expect_val = _s; _s

#define source(_str)

#define before_each  void ru_before_each_func()

__attribute__((weak)) void ru_before_each_func();



unsigned int    ru_test = 0;
unsigned int    ru_count_failure = 0;
char           *ru_tmp_str[RUNIT_ATTACH_COUNT];
char            ru_err_buf[1024] = "";
unsigned int    ru_namespace = 0;
char            ru_is_it = 0;
unsigned int    ru_expect_val;
bool            ru_is_expect = false;

int main(void)
{
    char* ru_str = "";
    #include "includes"
    printf("\n%d examples, %d failures\n", ru_test, ru_count_failure);
    return 0;
}
