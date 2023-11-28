#pragma once

#if defined(_WIN32)
    #define LPENGINE_PLATFORM_WINDOWS
    #ifndef _WIN64
        #error "LPENGINE only supports 64-bit Windows!"
    #endif // _WIN64
#elif defined(Linux)
    #define LPENGINE_PLATFORM_LINUX
    #ifndef __x86_64__
        #error "LPENGINE only supports 64-bit Linux!"
    #endif // __x86_64__
#else
    #error "LPENGINE only supports Windows and Linux!"
#endif // defined(_WIN32)

#ifdef LPENGINE_BUILD_DLL
    #ifdef LPENGINE_PLATFORM_WINDOWS
        #define LPENGINE_API __declspec(dllexport)
    #else
        #define LPENGINE_API __attribute__((visibility("default")))
    #endif // LPENGINE_PLATFORM_WINDOWS
#else
    #ifdef LPENGINE_PLATFORM_WINDOWS
        #define LPENGINE_API __declspec(dllimport)
    #else
        #define LPENGINE_API
    #endif // LPENGINE_PLATFORM_WINDOWS
#endif // LPENGINE_BUILD_DLL