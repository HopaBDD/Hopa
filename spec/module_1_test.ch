source("example_src/module_1.c")

{
    #include "../example_src/module_1.h"
    {
        /* This function is called implicitly before each "it" block. */
        before_each
        {
            ModuleOneInit();
        }

        context("With module One")
        {
            it("must initialize correctly")
            {
                ModuleOneGetState() to_eq(MOD_1_VAL);
            }
    
            describe("Modification")
            {
                it("should change the state correctly")
                {
                    ModuleOneChangeState(15);
    
                    ModuleOneGetState() to_eq(15);
                }
    
                it("should be a valid check for null")
                {
                    ModuleOneChangeState(0);
                    ModuleOneIsZero() to_eq(true);
    
                    ModuleOneChangeState(3);
                    ModuleOneIsZero() not_to_eq(true);
                }
            }
    
            it("should return the known string")
            {
                ModuleOneInit();
                char *ref_str = "Hello";
                char str[16];
    
                ModuleOneGetText(str);
    
                arr_to_eq(ref_str, str, 5);
            }
        }
    }
}
