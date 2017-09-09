#ifndef VMU_QUERY_HPP
#define VMU_QUERY_HPP

#include "region.hpp"
#include <system_error>

namespace vmu {

    inline local_region query(std::uintptr_t address);
    inline local_region query(std::uintptr_t address, std::error_code& ec) noexcept;

    inline local_region query(std::uintptr_t begin, std::uintptr_t end);
    inline local_region query(std::uintptr_t begin, std::uintptr_t end, std::error_code& ec) noexcept;


    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t address);
    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t address, std::error_code& ec) noexcept;

    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t begin, std::uintptr_t end);
    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uintptr_t begin, std::uintptr_t end, std::error_code& ec) noexcept;
}

#if defined(VMU_WINDOWS)
    #include "detail/windows/query.inl"
#elif defined(VMU_LINUX)
    #include "detail/linux/query.inl"
#else
    #include "detail/osx/query.inl"
#endif

#endif // !VMU_QUERY_HPP
