#pragma once

#ifdef _WIN32
    #ifdef RADIUMENGINE_EXPORTS
        #define RADIUM_API _declspec(dllexport) // Export when building the DLL
    #else
        #define RADIUM_API _declspec(dllimport) // Import when using the DLL
    #endif
#else
    #define RADIUM_API
#endif