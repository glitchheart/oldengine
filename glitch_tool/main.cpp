
#ifdef _WIN32
#include "windows.h"
#elif __linux || __APPLE__

#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#define GetProcAddress(handle, symbol) dlsym(handle, symbol)
#define LoadLibrary(path) dlopen(path, RTLD_LAZY)
#define GetLastError() errno

#include <string.h>

#endif

#include "stdio.h"
#include "common.h"
#include "project_main.h"
#include "install_main.h"
#include "build_main.h"
#include "run_main.h"

void LoadProjectCreator(char *FilePath, project_creator *Creator)
{
    Creator->CreatorDLL = LoadLibrary(FilePath);
    
    if (!Creator->CreatorDLL)
    {
        printf("Could not load project creator: %d\n", GetLastError());
    }
    Creator->CreateProject = (createproject *)GetProcAddress(Creator->CreatorDLL, "CreateProject");
}

void LoadInstaller(char *FilePath, glitch_installer *Installer)
{
    Installer->InstallerLib = LoadLibrary(FilePath);
    
    if (!Installer->InstallerLib)
    {
        printf("Could not load installer: %d\n", GetLastError());
    }
    
    Installer->InstallGlitch = (installglitch *)GetProcAddress(Installer->InstallerLib, "InstallGlitch");
}

void LoadBuilder(char *FilePath, glitch_builder *Builder)
{
    Builder->BuilderLib = LoadLibrary(FilePath);
    
    if (!Builder->BuilderLib)
    {
        printf("Could not load builder: %d\n", GetLastError());
    }
    
    Builder->BuildGlitch = (buildglitch *)GetProcAddress(Builder->BuilderLib, "BuildGlitch");
    Builder->ReleaseGame = (releasegame *)GetProcAddress(Builder->BuilderLib, "ReleaseGame");
}

void LoadRunner(char *FilePath, glitch_runner *Runner)
{
    Runner->RunnerLib = LoadLibrary(FilePath);
    
    if (!Runner->RunnerLib)
    {
        printf("Could not load runner: %d\n", GetLastError());
    }
    
    Runner->RunGlitch = (runglitch *)GetProcAddress(Runner->RunnerLib, "RunGlitch");
}

int main(int Argc, char **Args)
{
    project_creator Creator;
    glitch_installer Installer;
    glitch_builder Builder;
    glitch_runner Runner;
    
#ifdef _WIN32
    LoadProjectCreator("project.dll", &Creator);
    LoadInstaller("install.dll", &Installer);
    LoadBuilder("build.dll", &Builder);
    LoadRunner("run.dll", &Runner);
#elif __linux
    char Buf[155];
    i32 BufSize = 155;
    i32 Bytes = readlink("/proc/self/exe", Buf, BufSize);
    
    if (Bytes)
    {
        Buf[Bytes] = 0;
        char *SubBuf = (char *)malloc(strlen(Buf) - strlen("glitch"));
        
        strncpy(SubBuf, Buf, strlen(Buf) - strlen("glitch"));
        LoadProjectCreator(Concat(SubBuf, "libproject.so"), &Creator);
        LoadInstaller(Concat(SubBuf, "libinstall.so"), &Installer);
        LoadBuilder(Concat(SubBuf, "libbuild.so"), &Builder);
        LoadRunner(Concat(SubBuf, "librun.so"), &Runner);
        free(SubBuf);
    }
#elif __APPLE__
    auto EnginePath = Concat(getenv("GLITCH_ENGINE"), "/glitch_tool/build/");
    
    printf(Concat(EnginePath, "libproject.dylib"));
    LoadProjectCreator(Concat(EnginePath, "libproject.dylib"), &Creator);
    LoadInstaller(Concat(EnginePath, "libinstall.dylib"), &Installer);
    LoadBuilder(Concat(EnginePath, "libbuild.dylib"), &Builder);
    LoadRunner(Concat(EnginePath, "librun.dylib"), &Runner);
#endif
    
    if (Argc >= 2)
    {
        if (strcmp(Args[1], "install") == 0)
        {
            Installer.InstallGlitch();
        }
        else if (strcmp(Args[1], "build") == 0)
        {
            b32 WithEngine = false;
            
            if (Argc >= 3)
            {
                WithEngine = strcmp(Args[2], "-e") == 0;
            }
            
            Builder.BuildGlitch(WithEngine);
        }
        else if (strcmp(Args[1], "run") == 0)
        {
            b32 WithEngine = false;
            
            if (Argc >= 3)
            {
                WithEngine = strcmp(Args[2], "-e") == 0;
            }
            
            Runner.RunGlitch(WithEngine);
        }
        else if (strcmp(Args[1], "-p") == 0)
        {
            if (Argc >= 4)
            {
                if (strcmp(Args[2], "-n") == 0)
                {
                    Creator.CreateProject(Args[3]);
                }
            }
        }
        else if(strcmp(Args[1], "release") == 0)
        {
            auto WithZip = false;
            if(Argc >= 3 && strcpy(Args[2], "-z") == 0)
            {
                WithZip = true;
            }
            Builder.ReleaseGame(WithZip);
            
            
        }
        return 0;
    }
}
