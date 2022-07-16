#if OPERATING_SYSTEM == WINDOWS
#include <conio.h>
#else
#include <stdlib.h>
#endif // WIN32
#include "log.h"
#include "os.h"
#include "run.h"

int main(int argc, char * argv[])
{
        if (argc != 2) {
                logger(LOG_ERROR, "Expected argc to equal 2, got %d.\n", argc);
                goto PROGRAM_END;
        }

        char * fpath = argv[1];
        run(fpath);

PROGRAM_END:
        #if OPERATING_SYSTEM == WINDOWS
                logger(LOG_NONE, "Press any key to continue.\n");
                getch();
        #else
                logger(LOG_NONE, "Press enter to continue.\n");
                getchar();
        #endif
        return 0;
}
