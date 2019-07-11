#ifndef COMMON_H
#define COMMON_H

#include "stdlib.h"
#include "stdint.h"
#include "types.h"

inline char* Concat(const char* S1, const char* S2)
{
    char* Result;
    
    Result = (char*)malloc(strlen(S1) + strlen(S2) + 1);
    strcpy(Result, S1);
    strcat(Result, S2);
    return Result;
}

inline void WriteString(const char* S, FILE* File)
{
    fwrite(S, sizeof(char), strlen(S), File);
}

inline void WriteInclude(FILE* File)
{
    WriteString("#include ", File);
}

inline void WriteTab(FILE* File)
{
    WriteString("\t", File);
}

inline void WriteNewLine(FILE* File)
{
    WriteString("\n", File);
}

inline char* StrSep(char** S, const char* Delim)
{
    char* Start = *S;
    char* P;
    
    P = (Start != NULL) ? strpbrk(Start, Delim) : NULL;
    
    if(P == NULL)
    {
        *S = NULL;
    }
    else
    {
        *P = '\0';
        *S = P + 1;
    }
    return Start;
}

#ifdef __linux
static void CopyFile(const char* Src, const char* Dst, b32 Overwrite, b32 Binary = false)
{
    FILE* In;
    FILE* Out;
    
    (void)Overwrite;
    
    if(Binary)
    {
        In = fopen(Src, "rb");
    }
    else
    {
        In = fopen(Src, "r");
    }
    
    if(In == NULL)
    {
        printf("Failed in: %s\n", Src);
        return;
    }
    
    if(Binary)
    {
        Out = fopen(Dst, "wb");
    }
    else
    {
        Out = fopen(Dst, "w");
    }
    
    if(Out == NULL)
    {
        fclose(In);
        printf("Failed out\n");
        return;
    }
    
    size_t N,M;
    unsigned char Buff[8192];
    do
    {
        N = fread(Buff, 1, sizeof(Buff), In);
        if(N)
        {
            M = fwrite(Buff, 1, N, Out);
        }
        else
        {
            M = 0;
        }
    } while ((N > 0) && (N == M));
    if(M)
    {
        printf("COPY\n");
    }
    
    fclose(Out);
    fclose(In);
    
    if(Binary)
    {
        system(Concat("chmod +xr ", Dst));
    }
    
}

#elif __APPLE__
static void CopyFile(const char* Src, const char* Dst, b32 Overwrite, b32 Binary = false)
{
    FILE* In;
    FILE* Out;
    
    (void)Overwrite;
    
    if(Binary)
    {
        In = fopen(Src, "rb");
    }
    else
    {
        In = fopen(Src, "r");
    }
    
    if(In == NULL)
    {
        printf("Failed in: %s\n", Src);
        return;
    }
    
    if(Binary)
    {
        Out = fopen(Dst, "wb");
    }
    else
    {
        Out = fopen(Dst, "w");
    }
    
    if(Out == NULL)
    {
        fclose(In);
        printf("Failed out\n");
        return;
    }
    
    size_t N,M;
    unsigned char Buff[8192];
    do
    {
        N = fread(Buff, 1, sizeof(Buff), In);
        if(N)
        {
            M = fwrite(Buff, 1, N, Out);
        }
        else
        {
            M = 0;
        }
    } while ((N > 0) && (N == M));
    if(M)
    {
        printf("COPY\n");
    }
    
    fclose(Out);
    fclose(In);
    
    if(Binary)
    {
        system(Concat("chmod +xr ", Dst));
    }
    
}

#endif

#endif

