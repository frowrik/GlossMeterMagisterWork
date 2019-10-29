
#ifdef _WIN32
    #define    OS_WINDOW 1                     // Windows
    #ifdef _WIN64
        #define    OS_WIN64 1                  // Windows (64-bit only)
    #else
        #define    OS_WIN32 1                  // Windows (32-bit only)
    #endif
#elif __APPLE__
    ////#include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR                // iOS Simulator
        #error "Not used platform"
    #elif TARGET_OS_IPHONE
        #error "Not used platform"             // iOS device
    #elif TARGET_OS_MAC                        
        #error "Not used platform"             // Other kinds of Mac OS
    #else
        #error "Unknown Apple platform"
    #endif
#elif __linux__
    #define    OS_LINUX 1                      // linux  Debian 
    #define    OS_LINUX64  1
#elif __unix__
    #define    OS_UNUX 1                       // Unix
    #error "Not used platform"
#elif defined(_POSIX_VERSION)                  // POSIX
    #error "Not used platform"
#else
    #error "Unknown compiler"
#endif
