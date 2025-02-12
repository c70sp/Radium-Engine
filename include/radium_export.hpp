#pragma once

#ifdef _WIN32
    #ifdef RADIUMENGINE_EXPORTS
        #define RADIUM_API __declspec(dllexport) // Export when building the DLL
    #else
        #define RADIUM_API __declspec(dllimport) // Import when using the DLL
    #endif
#else
    #define RADIUM_API
#endif