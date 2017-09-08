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

#ifndef JM_PROTECTION_POSIX_PROTECT_MEMORY_HPP
#define JM_PROTECTION_POSIX_PROTECT_MEMORY_HPP

#include <cstdint>
#include <system_error>
#include <unistd.h>
#include <sys/mman.h>

namespace jm { namespace detail {

    using native_protection_t = int;

    inline void protect_memory(std::uintptr_t begin
                               , std::uintptr_t end
                               , native_protection_t native_prot)
    {
        static const auto page_size = ::sysconf(_SC_PAGESIZE);
        const auto address = (begin & -page_size);

        if(::mprotect(reinterpret_cast<void*>(address), end - begin, native_prot) == -1)
            throw std::system_error(std::error_code(errno, std::system_category())
                                    , "mprotect() failed");
    }


    inline void protect_memory(std::uintptr_t begin
                               , std::uintptr_t end
                               , native_protection_t native_prot
                               , std::error_code& ec) noexcept
    {
        static const auto page_size = ::sysconf(_SC_PAGESIZE);
        const auto address = (begin & -page_size);

        if (::mprotect(reinterpret_cast<void*>(address), end - begin, native_prot) == -1)
            ec = std::error_code(errno, std::system_category());
    }

}} // namespace jm::detail

#endif // !JM_PROTECTION_POSIX_PROTECT_MEMORY_HPP
