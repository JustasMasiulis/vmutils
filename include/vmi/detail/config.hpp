#ifndef JM_PROTECTION_GUARD_CONFIG_HPP
#define JM_PROTECTION_GUARD_CONFIG_HPP

#if defined(_WIN32)
    #define VMU_WINDOWS
#elif defined(__linux__)
    #define VMU_LINUX
#elif defined(__apple__)
    #define VMU_OSX
#else
    #error unsupported architecture
#endif 

#endif // !JM_PROTECTION_GUARD_CONFIG_HPP
