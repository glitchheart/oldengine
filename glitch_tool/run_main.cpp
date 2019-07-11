#include "run_main.h"
#include "stdio.h"
#ifdef _WIN32
#include "windows.h"
#include "Shlwapi.h"
#elif __linux || __APPLE__
#include <stdlib.h>
#endif
#include "files.h"

extern "C" RUNGLITCH(RunGlitch)
{
    // @Incomplete: Check if the file has been updated
    if(WithEngine)
        system("glitch build -e");
    else
        system("glitch build");
    
    printf("Running game...\n");
    
#ifdef _WIN32
    system("run.bat");
#elif __linux
    system("./run.sh");
#elif __APPLE__
    system("./osx_run.sh");
#endif
}

