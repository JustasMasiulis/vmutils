#ifndef VMU_WINDOWS_DEFINITIONS_HPP
#define VMU_WINDOWS_DEFINITIONS_HPP

extern "C" struct _SYSTEM_INFO;
extern "C" struct _MEMORY_BASIC_INFORMATION;

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

    struct MEMORY_BASIC_INFORMATION_ {
        void*         BaseAddress;
        void*         AllocationBase;
        unsigned long AllocationProtect;
        ULONG_PTR_    RegionSize;
        unsigned long State;
        unsigned long Protect;
        unsigned long Type;
    };

    struct SYSTEM_INFO_ {
        union {
            unsigned long dwOemId;
            struct {
                unsigned short wProcessorArchitecture;
                unsigned short wReserved;
            } info;
        };
        unsigned long  dwPageSize;
        void*          lpMinimumApplicationAddress;
        void*          lpMaximumApplicationAddress;
        ULONG_PTR_     dwActiveProcessorMask;
        unsigned long  dwNumberOfProcessors;
        unsigned long  dwProcessorType;
        unsigned long  dwAllocationGranularity;
        unsigned short wProcessorLevel;
        unsigned short wProcessorRevision;
    };

    extern "C" {

    __declspec(dllimport) unsigned long __stdcall GetLastError();

    __declspec(dllimport) ULONG_PTR_
        __stdcall VirtualQueryEx(void*                        handle,
                                 const void*                  address,
                                 ::_MEMORY_BASIC_INFORMATION* buffer,
                                 ULONG_PTR_                   size_of_info);

    __declspec(dllimport) int __stdcall VirtualProtect(
        void*          address,
        ULONG_PTR_     size,
        unsigned long  new_protection,
        unsigned long* old_protection);

    __declspec(dllimport) void __stdcall GetSystemInfo(::_SYSTEM_INFO* lpSystemInfo);

    __declspec(dllimport) void* __stdcall GetCurrentProcess();

    __declspec(dllimport) int __stdcall FlushInstructionCache(void*       handle,
                                                              const void* address,
                                                              ULONG_PTR_  size);
    }

    constexpr static unsigned long mem_commit  = 0x1000;
    constexpr static unsigned long mem_free    = 0x10000;
    constexpr static unsigned long mem_reserve = 0x2000;
    constexpr static unsigned long mem_private = 0x20000;
    constexpr static unsigned long page_guard  = 0x100;
    constexpr static unsigned long no_access   = 0x01;

}} // namespace vmu::detail

#endif // include guard
