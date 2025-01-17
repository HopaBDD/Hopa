# Behaviour Driven Development for C.

# hopa-core

hopa-core provides the structure for writing executable examples of how your code should behave, and an hopa command with tools to constrain which examples get run and tailor the output.

## Install

    echo alias hopa='./hopa/hopa/hopa' >> ~/.bashrc
    source ~/.bashrc

## Add in project

    cd your_project
    git clone git@github.com:HopaBDD/hopa.git
    hopa init

## Basic Structure

Hopa uses the words "describe" and "it" so we can express concepts like a conversation:

    "Describe a Modification"
    "It must initialize correctly"

```c
source("example_src/module_1.c")
{
    #include "../example_src/module_1.h"
    {
        describe("Modification")
        {
            it("must initialize correctly")
            {
                ModuleOneInit();
                
                ModuleOneGetState() to_eq(MOD_1_VAL);
            }
        }
    }
}
```

`before_each` block is called before each `it` block, and is intended for repetitive actions.

```c
{
    before_each
    {
        printf("Hello\n");
    }

    it("...") {}
    it("...") {}
    it("...") {}
}

Result:

Hello
Hello
Hello

```

## Nested Groups

You can also declare nested groups using the `describe` or `context` words:

```c
{
    context("with no items")
    {
        it("behaves one way")
        {
            /*
            ...
            */
        }
    }

    context("with one item")
    {
        it("behaves another way")
        {
            /*
            ...
            */
        }
    }
}
```

## The `hopa` Command

When you install the hopa-core , it installs the `hopa` script, which you'll use to run hopa. The `hopa` command comes with many useful options.
Run `hopa --help` to see the complete list.
