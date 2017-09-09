#ifndef VMU_WINDOWS_QUERY_INL
#define VMU_WINDOWS_QUERY_INL

#include "winapi.hpp"
#include "../../query.hpp"

namespace vmu {

    inline local_region query(std::uintptr_t address)
    {
        winapi::MEMORY_BASIC_INFORMATION info;
        if(winapi::VirtualQuery(reinterpret_cast<const void*>(address)
           , &info
           , sizeof(info) == 0)
           throw std::system_error
    }
    inline local_region query(std::uintptr_t address, std::error_code& ec) noexcept
    {}

    inline local_region query(std::uintptr_t begin, std::uintptr_t end)
    {}
    inline local_region query(std::uintptr_t begin, std::uintptr_t end, std::error_code& ec) noexcept
    {}


    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t address)
    {}
    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t address, std::error_code& ec) noexcept
    {}

    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t begin, std::uintptr_t end)
    {}
    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t begin, std::uintptr_t end, std::error_code& ec) noexcept
    {}
}

#endif // !VMU_WINDOWS_QUERY_INL
