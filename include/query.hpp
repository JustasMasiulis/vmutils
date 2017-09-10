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

#ifndef VMU_QUERY_HPP
#define VMU_QUERY_HPP

#include <vector>
#include <system_error>
#include "region.hpp"

namespace vmu {

    inline local_region query(std::uintptr_t address);
    inline local_region query(std::uintptr_t address, std::error_code& ec);

    inline std::vector<local_region> query_range(std::uintptr_t begin
                                                 , std::uintptr_t end);
    inline std::vector<local_region> query_range(std::uintptr_t begin
                                                 , std::uintptr_t end
                                                 , std::error_code& ec);


    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uint64_t address);
    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uint64_t address, std::error_code& ec);

    template<typename Handle>
    inline std::vector<remote_region> query_range(const Handle& handle
                                                  , std::uint64_t begin
                                                  , std::uint64_t end);
    template<typename Handle>
    inline std::vector<remote_region> query_range(const Handle& handle
                                                  , std::uint64_t begin
                                                  , std::uint64_t end
                                                  , std::error_code& ec);
}

#if defined(VMU_WINDOWS)
    #include "detail/windows/query.inl"
#elif defined(VMU_LINUX)
    #include "detail/linux/query.inl"
#else
    #include "detail/osx/query.inl"
#endif

#endif // !VMU_QUERY_HPP
