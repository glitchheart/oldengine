#ifndef INSTALLER_MAIN_H
#define INSTALLER_MAIN_H

#include "stdint.h"
#include "functions.h"
#include "types.h"

#ifdef _WIN32
#include "windows.h"
using module = HMODULE;
#elif __linux__ || __APPLE__
using module = void*;
#endif

struct glitch_installer
{
    module InstallerLib;
    installglitch* InstallGlitch;
};

#endif


