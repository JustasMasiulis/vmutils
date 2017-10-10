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

#include <system_error>
#include "protection.hpp"

namespace vmu {

    inline std::size_t page_size() noexcept;


    template<class Address>
    inline void protect(Address begin, Address end, protection_t prot);

    template<class Address>
    inline void protect(Address begin
                        , Address end
                        , protection_t prot
                        , std::error_code& ec);


    template<class Address>
    inline void protect(Address address, protection_t prot)
    {
        auto fixed_address = detail::cast_to_uintptr(address);
#ifdef _WIN32
        fixed_address &= -page_size();
#endif
        protect(fixed_address, fixed_address + 1, prot);
    }

    template<class Address>
    inline void protect(Address address, protection_t prot, std::error_code& ec)
    {
        auto fixed_address = detail::cast_to_uintptr(address);
#ifdef _WIN32
        fixed_address &= -page_size();
#endif
        protect(fixed_address, fixed_address + 1, prot, ec);
    }

} // namespace vmu

#if defined(_WIN32)
    #include "detail/windows/protect.inl"
#else
    #include "detail/posix/protect.inl"
#endif

#endif // include guard
