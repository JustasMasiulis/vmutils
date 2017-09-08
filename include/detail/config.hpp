#ifndef JM_PROTECTION_GUARD_CONFIG_HPP
#define JM_PROTECTION_GUARD_CONFIG_HPP

#if defined(_WIN32)
    #define JM_PG_WINDOWS
#elif defined(__linux__)
    #define JM_PG_LINUX
#elif defined(__apple__)
    #define JM_PG_APPLE
#else
    #error unsupported architecture
#endif 

#endif // !JM_PROTECTION_GUARD_CONFIG_HPP
