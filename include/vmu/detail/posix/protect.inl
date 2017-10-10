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

#ifndef VMU_POSIX_PROTECT_MEMORY_INL
#define VMU_POSIX_PROTECT_MEMORY_INL

#include "error.hpp"
#include "../../protect.hpp"
#include "../checked_pointers.hpp"
#include <unistd.h>
#include <sys/mman.h>

namespace vmu {

    inline std::size_t page_size() noexcept
    {
        // theoretically this may fail but don't think that this could happen in practice
        const static auto size = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
        return size;
    }

    template<class Address>
    inline void protect(Address begin, Address end, protection_t prot)
    {
        if (begin == end)
            return;

        const auto address = (detail::pointer_cast<std::uintptr_t>(begin) & -page_size());

        if (::mprotect(detail::pointer_cast_unchecked<void*>(address)
                       , detail::pointer_cast<std::size_t>(end) - address
                       , prot.native()) == -1)
            throw std::system_error(std::error_code(errno, std::system_category())
                                    , "mprotect() failed");
    }


    template<class Address>
    inline void
    protect(Address begin, Address end, protection_t prot, std::error_code& ec)
    {
        if(begin == end)
            return;

        const auto address = (detail::pointer_cast<std::uintptr_t>(begin) & -page_size());

        if (::mprotect(detail::pointer_cast_unchecked<void*>(address)
                       , detail::pointer_cast<std::size_t>(end) - address
                       , prot.native()) == -1)
            ec = std::error_code(errno, std::system_category());
    }

} // namespace vmu

namespace vmu { namespace detail {

    template<class Address>
    inline auto fix_singular_address(Address address)
    {
        return detail::cast_to_uintptr(address);
    }

}}

#endif // include guard
