#ifndef JM_PROTECTION_GUARD_HPP
#define JM_PROTECTION_GUARD_HPP

#include <cstdint>

namespace jm {

    enum class protection
    {
        none = 0,
        read = 1,
        write = 2,
        read_write = 3,
        exec = 4,
        read_exec = 5,
        write_exec = 6,
        read_write_exec = 7
    };

    template<typename Handle>
    protection protection_at(const Handle& handle, std::uintptr_t address)
    {

    }

    template<typename Handle>
    protection protection_at(const Handle& handle, std::uintptr_t address, std::error_code& ec)
    {

    }
}

#endif // !JM_PROTECTION_GUARD_HPP
