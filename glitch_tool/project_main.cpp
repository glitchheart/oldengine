#include "project_main.h"
#include "stdio.h"
#include "stdlib.h"

#ifdef _WIN32
#include "windows.h"
#include "Shlwapi.h"

#elif __linux
#include <dirent.h>
#include <sys/stat.h>  
#include <sys/sendfile.h>
#include "unistd.h"
#include <string.h>
#include <fcntl.h>

#define PathFileExists(name) (access(name, F_OK) != -1)

int RemoveDirectory(const char *Path)
{
    DIR *D = opendir(Path);
    size_t PathLen = strlen(Path);
    int R = -1;
    
    if (D)
    {
        struct dirent *P;
        
        R= 0;
        
        while (!R && (P=readdir(D)))
        {
            int R2 = -1;
            char *Buf;
            size_t Len;
            
            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(P->d_name, ".") || !strcmp(P->d_name, ".."))
            {
                continue;
            }
            
            Len = PathLen + strlen(P->d_name) + 2; 
            Buf = (char*)malloc(Len);
            
            if (Buf)
            {
                struct stat StatBuf;
                
                snprintf(Buf, Len, "%s/%s", Path, P->d_name);
                
                if (!stat(Buf, &StatBuf))
                {
                    if (S_ISDIR(StatBuf.st_mode))
                    {
                        R2 = RemoveDirectory(Buf);
                    }
                    else
                    {
                        R2 = unlink(Buf);
                    }
                }
                
                free(Buf);
            }
            
            R= R2;
        }
        
        closedir(D);
    }
    
    if (!R)
    {
        R= rmdir(Path);
    }
    
    return R;
}

#elif __APPLE__

#include <dirent.h>
#include <sys/stat.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include "unistd.h"
#include <string.h>
#include <fcntl.h>

#define PathFileExists(name) (access(name, F_OK) != -1)

int RemoveDirectory(const char *Path)
{
    DIR *D = opendir(Path);
    size_t PathLen = strlen(Path);
    int R = -1;
    
    if (D)
    {
        struct dirent *P;
        
        R= 0;
        
        while (!R && (P=readdir(D)))
        {
            int R2 = -1;
            char *Buf;
            size_t Len;
            
            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(P->d_name, ".") || !strcmp(P->d_name, ".."))
            {
                continue;
            }
            
            Len = PathLen + strlen(P->d_name) + 2; 
            Buf = (char*)malloc(Len);
            
            if (Buf)
            {
                struct stat StatBuf;
                
                snprintf(Buf, Len, "%s/%s", Path, P->d_name);
                
                if (!stat(Buf, &StatBuf))
                {
                    if (S_ISDIR(StatBuf.st_mode))
                    {
                        R2 = RemoveDirectory(Buf);
                    }
                    else
                    {
                        R2 = unlink(Buf);
                    }
                }
                
                free(Buf);
            }
            
            R= R2;
        }
        
        closedir(D);
    }
    
    if (!R)
    {
        R= rmdir(Path);
    }
    
    return R;
}

#endif

#include "files.h"
#include "common.h"

extern "C" CREATEPROJECT(CreateProject)
{
    if(Name)
    {
        b32 Write = true;
        if(PathFileExists(Name))
        {
            printf("Folder already exists, overwrite?\n");
            printf("y: Yes\n");
            printf("n: No\n");
            char Input[32];
            if(fgets(Input, sizeof(Input), stdin))
            {
                if(strcmp(Input, "y\n") == 0)
                {
                    RemoveDirectory(Name);
                }
                else
                {
                    Write = false;
                }
            }
            
        }
        
        if(Write)
        {
#ifdef _WIN32
            CreateDirectoryA(Name, 0);
            CreateDirectoryA(Concat(Name, "/src"), 0);
#elif __linux || __APPLE__ 
            mkdir(Name, 0700);
            mkdir(Concat(Name, "/src"), 0700);
#endif
            
            //Write build.bat
            FILE* File = fopen(Concat(Name, "/build.bat"), "w");
            
            if(File)
            {
                WriteString(BuildBat, File);
                fclose(File);
            }
            
            File = fopen(Concat(Name, "/build.sh"), "w");
            if(File)
            {
                WriteString(BuildSh, File);
                system(Concat("chmod +xr ", Concat(Name, "/build.sh")));
                fclose(File);
            }
            
            File = fopen(Concat(Name, "/osx_build.sh"), "w");
            if(File)
            {
                WriteString(BuildShOSX, File);
                system(Concat("chmod +xr ", Concat(Name, "/osx_build.sh")));
                fclose(File);
            }
            
            //Write actual_build.bat
            File = fopen(Concat(Name, "/actual_build.bat"), "w");
            if(File)
            {
                WriteString(ActualBuildBat, File);
                fclose(File);
            }
            
            File = fopen(Concat(Name, "/actual_build.sh"), "w");
            if(File)
            {
                WriteString(ActualBuildSh, File);
                system(Concat("chmod +xr ", Concat(Name, "/actual_build.sh")));
                fclose(File);
            }
            
            File = fopen(Concat(Name, "/osx_actual_build.sh"), "w");
            if(File)
            {
                WriteString(ActualBuildShOSX, File);
                system(Concat("chmod +xr ", Concat(Name, "/osx_actual_build.sh")));
                fclose(File);
            }
            //Write timecmd
            
            File = fopen(Concat(Name, "/timecmd.bat"), "w");
            
            if(File)
            {
                WriteString(TimeCmd, File);
                fclose(File);
            }
            
            //Write run.bat
            File = fopen(Concat(Name, "/run.bat"), "w");
            if(File)
            {
                WriteString(RunBat, File);
                fclose(File);
            }
            
            File = fopen(Concat(Name, "/run.sh"), "w");
            if(File)
            {
                WriteString(RunSh, File);
                system(Concat("chmod +xr ", Concat(Name, "/run.sh")));
                fclose(File);
            }
            
            File = fopen(Concat(Name, "/osx_run.sh"), "w");
            if(File)
            {
                WriteString(RunShOSX, File);
                system(Concat("chmod +xr ", Concat(Name, "/osx_run.sh")));
                fclose(File);
            }
            // Write game.h
#ifdef _WIN32
            File = fopen(Concat(Name, "\\src\\game.h"), "w");
#elif __linux || __APPLE__
            File = fopen(Concat(Name, "/src/game.h"), "w");
#endif
            if(File)
            {
                WriteString("#ifndef GAME_H\n", File);
                WriteString("#define GAME_H\n", File);
                
                WriteNewLine(File);
                
                for(i32 H = 0; H < GAME_HEADER_INCLUDES; H++)
                {
                    WriteInclude(File);
                    WriteString(GameHeaderIncludes[H], File);
                    WriteNewLine(File);
                }
                
                WriteNewLine(File);
                WriteString("struct game_state\n", File);
                WriteString("{", File);
                WriteNewLine(File);
                
                
                for(i32 H = 0; H < GAME_STATE_FIELDS; H++)
                {
                    WriteTab(File);
                    WriteString(GameStateFields[H], File);
                    WriteNewLine(File);
                }
                
                WriteString("};", File);
                WriteNewLine(File);
                WriteNewLine(File);
                
                WriteString("#endif", File);
                
                fclose(File);
            }
            
            // Write game.cpp
#ifdef _WIN32
            File = fopen(Concat(Name, "\\src\\game.cpp"), "w");
#elif __linux || __APPLE__
            File = fopen(Concat(Name, "/src/game.cpp"), "w");
#endif
            if(File)
            {
                
                for(i32 H = 0; H < GAME_CPP_INCLUDES; H++)
                {
                    WriteInclude(File);
                    WriteString(GameCppIncludes[H], File);
                    WriteNewLine(File);
                }
                
                WriteString("platform_api Platform;\n", File);
                WriteNewLine(File);
                
                WriteString(InitUpdate, File);
                fclose(File);
            }
            
            char* EnginePath = getenv("GLITCH_ENGINE");
            if(EnginePath)
            {
#ifdef _WIN32
                CreateDirectoryA(Concat(Name, "/build"), 0);
                CopyFile(Concat(EnginePath, "/build/main.exe"), Concat(Name, "/build/main.exe"), false);
                CopyFile(Concat(EnginePath, "/build/main.pdb"), Concat(Name, "/build/main.pdb"), false);
                CopyFile(Concat(EnginePath, "/build/main.map"), Concat(Name, "/build/main.map"), false);
                
                File = fopen(Concat(Name, "/.config"), "w");
                WriteString(ConfigFile, File);
                fclose(File);
                
                printf("Copying file\n");
                CopyFile(Concat(EnginePath, "/.gitignore"), Concat(Name, "/.gitignore"), false);
#elif __linux || __APPLE__
                char DirectoryBuffer[256];
                getcwd(DirectoryBuffer, 256);
                
                mkdir(Concat(Name, "/build"), 0700);
                CopyFile(Concat(EnginePath, "/build/main"), Concat(Name, "/build/main"), false,true);
                
                File = fopen(Concat(Name, "/.config"), "w");
                WriteString(ConfigFile, File);
                fclose(File);
                
                printf("Copying file\n");
                CopyFile(Concat(EnginePath, "/.gitignore"), Concat(Name, "/.gitignore"), false);
#endif
            }
        }
    }
}