/*
* Copyright 2017 Justas Masiulis
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef VMU_POSIX_PROTECTION_INL
#define VMU_POSIX_PROTECTION_INL

#include "../../protection.hpp"

namespace vmu {

    constexpr inline native_protection_t to_native(access flags)
    {
        return static_cast<native_protection_t>(access);
    }

    constexpr inline access from_native(native_protection_t my_flags)
    {
        return static_cast<access>(my_flags);
    }

    constexpr bool protection_t::accessible() const noexcept
    {
        return _native != 0;
    }

    constexpr bool protection_t::readable() const noexcept
    {
        return (_native & static_cast<native_protection_t>(access::read));
    }

    constexpr bool protection_t::writable() const noexcept
    {
        return (_native & static_cast<native_protection_t>(access::write));
    }

    constexpr bool protection_t::executable() const noexcept
    {
        return (_native & static_cast<native_protection_t>(access::exec));
    }

} // namespace vmu

#endif // include guard
