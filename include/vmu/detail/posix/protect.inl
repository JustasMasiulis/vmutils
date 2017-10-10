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

    template<class Address>
    inline void protect(Address begin, Address end, protection::storage prot)
    {
        static const auto page_size = ::sysconf(_SC_PAGESIZE);
        const auto        address   = (detail::pointer_cast<std::uintptr_t>(begin) & -page_size);

        if (::mprotect(detail::pointer_cast_unchecked<void*>(address)
                       , detail::pointer_cast_unchecked<std::size_t>(detail::ptr_distance(end, begin))
                       , prot.native()) == -1)
            throw std::system_error(std::error_code(errno, std::system_category()), "mprotect() failed");
    }


    template<class Address>
    inline void
    protect(Address begin, Address end, protection::storage prot, std::error_code& ec)
    {
        static const auto page_size = ::sysconf(_SC_PAGESIZE);
        const auto address = (detail::pointer_cast<std::uintptr_t>(begin) & -page_size);

        if (::mprotect(detail::pointer_cast_unchecked<void*>(address)
                       , detail::pointer_cast_unchecked<std::size_t>(detail::ptr_distance(end, begin))
                       , prot.native()) == -1)
            ec = std::error_code(errno, std::system_category());
    }

} // namespace vmu

#endif // include guard
