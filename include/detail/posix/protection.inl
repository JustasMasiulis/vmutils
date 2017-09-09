#ifndef VMU_POSIX_PROTECTION_INL
#define VMU_POSIX_PROTECTION_INL

#include "../../protection.hpp"
#include <stdexcept>
#include <string>

namespace vmu { namespace protection {

    constexpr inline native_protection_t to_native(flags flags)
    {
        return static_cast<native_protection_t>(flags);
    }

    constexpr inline flags from_native(native_protection_t my_flags)
    {
        return static_cast<flags>(my_flags);
    }

    constexpr bool storage::accessible() const noexcept
    {
        return _native != 0;
    }

    constexpr bool storage::readable() const noexcept
    {
        return (_native & static_cast<native_protection_t>(flags::read));
    }

    constexpr bool storage::writable() const noexcept
    {
        return (_native & static_cast<native_protection_t>(flags::write));
    }

    constexpr bool storage::executable() const noexcept
    {
        return (_native & static_cast<native_protection_t>(flags::exec));
    }

}} // namespace vmu::protection

#endif // !VMU_POSIX_PROTECTION_INL
