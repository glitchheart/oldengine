#include "build_main.h"
#include "stdio.h"
#ifdef _WIN32
#include "windows.h"
#include "Shlwapi.h"
#elif __linux
#include <sys/stat.h>
#include <sys/sendfile.h>
#include "unistd.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#elif __APPLE__
#include <sys/stat.h>

// @Incomplete: TEMPORARY FOR INCLUDING THE SAME AS LINUX with sendfile.h. Check later if all are necessary
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include "unistd.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#endif

#include "files.h"
#include "common.h"

void CompileEngine()
{
    printf("Compiling engine....\n");
    char* EnginePath = getenv("GLITCH_ENGINE");
#ifdef _WIN32
    system(Concat(EnginePath, "\\build.bat"));
#elif __linux
    system(Concat(EnginePath, "/build.sh"));
#elif __APPLE__
    system(Concat(EnginePath, "/osx_build.sh"));
#endif
}

void CompileGame()
{
#ifdef _WIN32
    system("build.bat");
#elif __linux
    system("./build.sh");
#elif __APPLE__
    system("./osx_build.sh");
#endif
}

void CopyEngine(const char* EnginePath, const char* DirectoryBuffer, b32 Debug = false)
{
#ifdef _WIN32
    CreateDirectoryA(Concat(DirectoryBuffer, "/build"), 0);
    CopyFile(Concat(EnginePath, "/build/altered.exe"), Concat(DirectoryBuffer, "/build/altered.exe"), false);
    
    if(Debug)
    {
        CopyFile(Concat(EnginePath, "/build/altered.pdb"), Concat(DirectoryBuffer, "/build/altered.pdb"), false);
        CopyFile(Concat(EnginePath, "/build/altered.map"), Concat(DirectoryBuffer, "/build/altered.map"), false);
        
    }
    CopyFile(Concat(EnginePath, "/libs/fmod/dll/fmod64.dll"), Concat(DirectoryBuffer, "/build/fmod64.dll"), false);
#elif __linux
    mkdir(Concat(DirectoryBuffer, "/build"), 0700);
    CopyFile(Concat(EnginePath, "/build/altered"), Concat(DirectoryBuffer, "/build/altered"), false, true);
    CopyFile(Concat(EnginePath, "/build/libfmod.so.10.1"), Concat(DirectoryBuffer, "/build/libfmod.so.10.1"), false, true);
    
    CopyFile(Concat(EnginePath, "/build/libglad.so"), Concat(DirectoryBuffer, "/build/libglad.so"), false, true);
    CopyFile(Concat(EnginePath, "/build/libglfw.so.3.2"), Concat(DirectoryBuffer, "/build/libglfw.so.3.2"), false, true);
    system(Concat("rm ", Concat(DirectoryBuffer,"/build/libfmod.so.10")));
    system(Concat("rm ", Concat(DirectoryBuffer,"/build/libglfw.so.3")));
    system(Concat("rm ", Concat(DirectoryBuffer,"/build/libglfw.so")));
    
    system(Concat("ln -s ", Concat(Concat(DirectoryBuffer,"/build/libfmod.so.10.1 "), Concat(DirectoryBuffer, "/build/libfmod.so.10"))));
    system(Concat("ln -s ", Concat(Concat(DirectoryBuffer,"/build/libglfw.so.3.2 "), Concat(DirectoryBuffer, "/build/libglfw.so.3"))));
    system(Concat("ln -s ", Concat(Concat(DirectoryBuffer,"/build/libglfw.so.3 "), Concat(DirectoryBuffer, "/build/libglfw.so"))));
#elif __APPLE__
    mkdir(Concat(DirectoryBuffer, "/build"), 0700);
    CopyFile(Concat(EnginePath, "/build/altered"), Concat(DirectoryBuffer, "/build/altered"), false, true);
    CopyFile(Concat(EnginePath, "/libs/fmod/lib/osx/libfmod.dylib"), Concat(DirectoryBuffer, "/build/libfmod.dylib"), false, true);
    
    //CopyFile(Concat(EnginePath, "/build/libglad.so"), Concat(DirectoryBuffer, "/build/libglad.so"), false, true);
    //CopyFile(Concat(EnginePath, "/build/libglfw.so.3.2"), Concat(DirectoryBuffer, "/build/libglfw.so.3.2"), false, true);
    //system(Concat("rm ", Concat(DirectoryBuffer,"/build/libfmod.so.10")));
    //system(Concat("rm ", Concat(DirectoryBuffer,"/build/libglfw.so.3")));
    //system(Concat("rm ", Concat(DirectoryBuffer,"/build/libglfw.so")));
    //printf("%s\n", Concat("ln -s ", Concat(Concat(DirectoryBuffer,"/build/libfmod.so.10.1 "), Concat(DirectoryBuffer, "/build/libfmod.so.10"))));
    //system(Concat("ln -s ", Concat(Concat(DirectoryBuffer,"/build/libfmod.so.10.1 "), Concat(DirectoryBuffer, "/build/libfmod.so.10"))));
    //system(Concat("ln -s ", Concat(Concat(DirectoryBuffer,"/build/libglfw.so.3.2 "), Concat(DirectoryBuffer, "/build/libglfw.so.3"))));
    //system(Concat("ln -s ", Concat(Concat(DirectoryBuffer,"/build/libglfw.so.3 "), Concat(DirectoryBuffer, "/build/libglfw.so"))));
#endif
}

void CopyAssets(const char* EnginePath, const char* DirectoryBuffer)
{
    printf("Copying engine assets...");
    // Copy all shaders
    char Buffer[512];
#ifdef _WIN32
    sprintf(Buffer, "xcopy %s %s /c /s /e /i /y>nul 2>nul", Concat(EnginePath, "\\engine_assets\\shaders"), Concat(DirectoryBuffer, "\\engine_assets\\shaders"));
    system(Buffer);
    sprintf(Buffer, "xcopy %s %s /c /s /e /i /y>nul 2>nul", Concat(EnginePath, "\\engine_assets\\standard_shaders"), Concat(DirectoryBuffer, "\\engine_assets\\standard_shaders"));
    system(Buffer);
#elif __linux || __APPLE__
    char rm_buffer[512];
    sprintf(rm_buffer, "rm -rf %s 2>&1 > /dev/null", Concat(DirectoryBuffer, "/engine_assets"));
    system(rm_buffer);
    sprintf(Buffer, "cp -Rf %s %s 2>&1 > /dev/null", Concat(EnginePath, "/engine_assets"), Concat(DirectoryBuffer, "/engine_assets"));
#endif

}

void CopyGame(const char* OriginalDir, const char* NewDir)
{
    char Buffer[512];
    
#ifdef _WIN32
    CopyFile(Concat(OriginalDir, "/build/game.dll"), Concat(NewDir, "/build/game.dll"), false);
    CopyFile(Concat(OriginalDir, "/.config"), Concat(NewDir, "/.config"), false);
    sprintf(Buffer, "xcopy %s %s /c /s /e /i /y>nul 2>nul", Concat(OriginalDir, "\\assets"), Concat(NewDir, "\\assets"));
#elif __linux
    
    CopyFile(Concat(OriginalDir, "/build/libgame.so"), Concat(NewDir, "/build/libgame.so"), false, true);
    CopyFile(Concat(OriginalDir, "/.config"), Concat(NewDir, "/.config"), false, true);
    sprintf(Buffer, "cp -R %s %s 2>&1 /dev/null", Concat(OriginalDir, "/assets"), Concat(NewDir, "/assets"));
#elif __APPLE__
    CopyFile(Concat(OriginalDir, "/build/libgame.dlyb"), Concat(NewDir, "/build/libgame.dlyb"), false, true);
    CopyFile(Concat(OriginalDir, "/.config"), Concat(NewDir, "/.config"), false, true);
    sprintf(Buffer, "cp -R %s %s 2>&1 /dev/null", Concat(OriginalDir, "/assets"), Concat(NewDir, "/assets"));
#endif
    system(Buffer);
}

extern "C" RELEASEGAME(ReleaseGame)
{
    CompileEngine();
    CompileGame();
    char DirectoryBuffer[256];
#ifdef _WIN32
    GetCurrentDirectory(256, DirectoryBuffer);
#elif __linux || __APPLE__
    getcwd(DirectoryBuffer, 256);
#endif
    
    auto Dir = Concat(DirectoryBuffer, "\\release");
    printf("%s\n", Dir);
#ifdef _WIN32
    CreateDirectoryA(Dir, 0);
#elif __linux || __APPLE__
    mkdir(Dir, 0700);
#endif
    char* EnginePath = getenv("GLITCH_ENGINE");
    if(EnginePath)
    {
        CopyEngine(EnginePath, Dir);
        //CopyAssets(EnginePath, Dir);
        
        CopyGame(DirectoryBuffer, Dir);
    }
    if(WithZip)
    {
    }
}

extern "C" BUILDGLITCH(BuildGlitch)
{
    char* EnginePath = getenv("GLITCH_ENGINE");

#if defined(__linux) || defined(__APPLE__)
    char cwf_buf[512];
    getcwd(cwf_buf, 512);
    if(strcmp(cwf_buf, EnginePath) == 0)
    {
        return;
    }
#endif
    if(WithEngine)
    {
        CompileEngine();
    }
    
    CompileGame();
    
    
    if(EnginePath)
    {
        char DirectoryBuffer[256];
#ifdef _WIN32
        GetCurrentDirectory(256, DirectoryBuffer);
#elif __linux || __APPLE__
        getcwd(DirectoryBuffer, 256);
#endif
        CopyEngine(EnginePath, DirectoryBuffer);
//        CopyAssets(EnginePath, DirectoryBuffer);
    }
}

