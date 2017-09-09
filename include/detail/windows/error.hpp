#ifndef VMU_WINDOWS_ERROR_HPP
#define VMU_WINDOWS_ERROR_HPP

#include <system_error>
#include "winapi.hpp"

namespace vmu { namespace detail {

    inline std::error_code get_last_error() noexcept
    {
        return std::error_code(static_cast<int>(winapi::GetLastError())
                               , std::system_category());
    }

    inline void throw_last_error(const char* msg)
    {
        throw std::system_error(get_last_error(), msg);
    }

}}

#endif // !VMU_WINDOWS_ERROR_HPP