#ifndef MAIN_H
#define MAIN_H

#include "stdint.h"
#include "functions.h"
#include "types.h"

#ifdef _WIN32
#include "windows.h"
using module = HMODULE;
#elif __linux || __APPLE__
using module = void*;
#endif

struct project_creator
{
    module CreatorDLL;
    createproject* CreateProject;
};


#endif

