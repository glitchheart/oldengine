#include "install_main.h"
#include "stdio.h"
#include "stdlib.h"
#include "files.h"
#include "string.h"
#include "util.h"


extern "C" INSTALLGLITCH(InstallGlitch)
{
#ifdef _WIN32
    system("..\\install.bat");
#elif __linux
    system("../install.sh");
#endif
}