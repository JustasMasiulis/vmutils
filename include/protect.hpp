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

#ifndef VMU_PROTECT_MEMORY_HPP
#define VMU_PROTECT_MEMORY_HPP

#include <cstdint>
#include "protection.hpp"

namespace vmu {

    inline void protect(std::uintptr_t begin
                               , std::uintptr_t end
                               , protection::storage prot);
    template<typename Range>
    inline void protect(const Range& range, protection::storage prot)
    {
        protect_memory(range.begin, range.end, prot);
    }


    inline void protect(std::uintptr_t begin
                               , std::uintptr_t end
                               , protection::storage prot
                               , std::error_code& ec) noexcept;

    template<typename Range>
    inline void protect(const Range& range
                               , protection::storage prot
                               , std::error_code& ec) noexcept
    {
        protect_memory(range.begin, range.end, prot, ec);
    }

} // namespace vmu

#if defined(VMU_WINDOWS)
    #include "detail/windows/protect.inl"
#else
    #include "detail/posix/protect.inl"
#endif

#endif // !VMU_PROTECT_MEMORY_HPP
