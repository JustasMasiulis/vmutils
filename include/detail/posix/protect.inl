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

#include "../../protect.hpp"
#include <system_error>
#include <unistd.h>
#include <sys/mman.h>

namespace vmu {

    inline void protect(std::uintptr_t begin
                        , std::uintptr_t end
                        , protection::storage prot)
    {
        static const auto page_size = ::sysconf(_SC_PAGESIZE);
        const auto address = (begin & -page_size);

        if (::mprotect(reinterpret_cast<void*>(address), end - begin, prot.native()) == -1)
            throw std::system_error(std::error_code(errno, std::system_category())
                                    , "mprotect() failed");
    }


    inline void protect(std::uintptr_t begin
                        , std::uintptr_t end
                        , protection::storage prot
                        , std::error_code& ec) noexcept
    {
        static const auto page_size = ::sysconf(_SC_PAGESIZE);
        const auto address = (begin & -page_size);

        if (::mprotect(reinterpret_cast<void*>(address), end - begin, prot.native()) == -1)
            ec = std::error_code(errno, std::system_category());
    }

} // namespace vmu

#endif // !VMU_POSIX_PROTECT_MEMORY_INL
