#ifndef VMU_WINDOWS_DEFINITIONS_HPP
#define VMU_WINDOWS_DEFINITIONS_HPP

namespace vmu { namespace detail {

#ifdef _WIN64
    #if defined(__CYGWIN__)
    typedef unsigned long ULONG_PTR_;
    #else
    typedef unsigned __int64 ULONG_PTR_;
    #endif
#else
    typedef unsigned long ULONG_PTR_;
#endif

    struct MEMORY_BASIC_INFORMATION {
        void* BaseAddress;
        void* AllocationBase;
        unsigned long AllocationProtect;
        ULONG_PTR_    RegionSize;
        unsigned long State;
        unsigned long Protect;
        unsigned long Type;
    };

    extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();

    extern "C" __declspec(dllimport) ULONG_PTR_ __stdcall
    VirtualQuery(const void* address, MEMORY_BASIC_INFORMATION* buffer, ULONG_PTR_ size_of_info);

    constexpr static unsigned long mem_commit  = 0x1000;
    constexpr static unsigned long mem_free    = 0x10000;
    constexpr static unsigned long mem_reserve = 0x2000;
    constexpr static unsigned long mem_private = 0x20000;

}} // namespace vmu::detail

#endif // include guard
