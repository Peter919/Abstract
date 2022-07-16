#include "stack.h"
#include "compiler.h"
#include "execute.h"

char run(char * fpath)
{
        struct Stack stack = compile(fpath);
        if (stack.max_size == 0) {
                return 0;
        }

        execute(&stack);
        return 1;
}
