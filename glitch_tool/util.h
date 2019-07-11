
inline char* Concat(const char* S1, const char* S2)
{
    char* Result;
    
    Result = (char*)malloc(strlen(S1) + strlen(S2) + 1);
    strcpy(Result, S1);
    strcat(Result, S2);
    return Result;
}


