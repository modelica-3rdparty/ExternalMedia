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
T tryImportSymbol(const char *funcName)
{
    /* TODO: we should do caching */
    
    /* First check if the executable itself exports the symbol we want */
    HMODULE exe = GetModuleHandleA(NULL);
    if(exe == NULL)
    {
        fprintf(stderr, "Can't get handle to executable (error %d)\n", GetLastError());
        exit(1);
    }
    T pfn = reinterpret_cast<T>(GetProcAddress(exe, funcName));
    if(pfn) return pfn;
    
    /* If we don't find it in the executable, then we search it in all loaded DLLs */
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

    int num_modules = cbNeeded / sizeof(HMODULE); /* Actual number of loaded modules */
    for(int i = 0; i < num_modules; i++)
    {
        T pfn = reinterpret_cast<T>(GetProcAddress(loaded_modules[i], funcName));
        if(pfn) return pfn;
    }

    /* not found */
    return NULL;
}

template<typename T>
T importSymbol(const char *funcName)
{
    T result = tryImportSymbol<T>(funcName);
    if(result) return result;

    fprintf(stderr, "Can't get handle to %s in all loaded modules.\n", funcName);
    exit(1);
}

#define IMPORT(x,y) auto y = importSymbol<x>(#y)

#else /* _WIN32 */

#include "ModelicaUtilities.h"

/*  Nothing to do on Linux, its linker just works */
#define IMPORT(x,y)

#endif /* _WIN32 */
