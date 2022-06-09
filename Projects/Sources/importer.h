/* *****************************************************************
 * Working around Windows' dynamic linker
 *
 * Federico Terraneo, Mahder Gebremedhin May 2022
 ********************************************************************/

#pragma once

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <libloaderapi.h>
#include <errhandlingapi.h>
#include <psapi.h>

template<typename T>
T importFromExecutable(const char *funcName)
{
    //TODO: if GetModuleHandleA/GetProcAddress are heavy we could do caching
    HMODULE exe = GetModuleHandleA(NULL);
    if(exe == NULL)
    {
        fprintf(stderr, "Can't get handle to executable (error %d)\n", GetLastError());
        exit(1);
    }
    T pfn = reinterpret_cast<T>(GetProcAddress(exe, funcName));
    if(pfn == NULL)
    {
        fprintf(stderr, "Can't get handle to %s (error %d)\n",funcName, GetLastError());
        exit(1);
    }
    return pfn;
}

template<typename T>
T importFromLoadedModules(const char *funcName)
{
    //TODO: we should do caching
    HANDLE process = GetCurrentProcess();
    if(process == NULL)
    {
        fprintf(stderr, "Can't get a handle to current process (error %d)\n", GetLastError());
        exit(1);
    }

    HMODULE loaded_modules[1024];
    DWORD cbNeeded;
    auto result = EnumProcessModules(process, loaded_modules, sizeof(loaded_modules), &cbNeeded);
    CloseHandle(process);
    if(!result)
    {
        fprintf(stderr, "Can't enumerate loaded modules (error %d)\n", GetLastError());
        exit(1);
    }

    // The actual number of loaded modules.
    int num_modules = cbNeeded / sizeof(HMODULE);
    for(int i = 0; i < num_modules; i++)
    {
        T pfn = reinterpret_cast<T>(GetProcAddress(loaded_modules[i], funcName));
        if(pfn) return pfn;
    }

    fprintf(stderr, "Can't get handle to %s in all loaded modules.\n", funcName);
    exit(1);
}

#define IMPORT(x,y) auto y = importFromLoadedModules<x>(#y)

#else //_WIN32

#include "ModelicaUtilities.h"

// Nothing to do on Linux, its linker just works
#define IMPORT(x,y)

#endif //_WIN32
