#ifndef VMU_WINDOWS_PROTECTION_INL
#define VMU_WINDOWS_PROTECTION_INL

#include "../../protection.hpp"

namespace vmu { namespace protection {


    inline native_protection_t to_native(flags flags)
    {

    }

    inline flags from_native(native_protection_t flags)
    {

    }

    bool storage::accessible() const noexcept
    {}
    bool storage::readable() const noexcept
    {}
    bool storage::writable() const noexcept
    {}
    bool storage::executable() const noexcept
    {}

    flags storage::to_flags() const
    {}

}} // namespace vmu::protection

#endif // !VMU_WINDOWS_PROTECTION_INL
