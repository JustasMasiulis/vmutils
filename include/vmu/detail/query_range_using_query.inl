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

#ifndef VMU_QUERY_RANGE_USING_QUERY_INL
#define VMU_QUERY_RANGE_USING_QUERY_INL

#include "../../query.hpp"

namespace vmu {

    inline std::vector<local_region> query_range(std::uintptr_t begin, std::uintptr_t end)
    {
        std::vector<local_region> regions;
        while (begin < end) {
            regions.emplace_back(query(begin));
            begin = regions.back().end();
        }

        return regions;
    }
    inline std::vector<local_region>
    query_range(std::uintptr_t begin, std::uintptr_t end, std::error_code& ec)
    {
        std::vector<local_region> regions;
        while (begin < end) {
            regions.emplace_back(query(begin, ec));
            if (ec)
                return regions;

            begin = regions.back().end();
        }

        return regions;
    }

    template<typename Handle>
    inline std::vector<remote_region>
    query_range(Handle handle, std::uint64_t begin, std::uint64_t end)
    {
        std::vector<local_region> regions;
        while (begin < end) {
            regions.emplace_back(query(handle, begin));
            if (ec)
                return regions;

            begin = regions.back().end();
        }

        return regions;
    }
    template<typename Handle>
    inline std::vector<remote_region> query_range(Handle handle
                                                  , std::uint64_t begin
                                                  , std::uint64_t end
                                                  , std::error_code& ec)
    {
        std::vector<local_region> regions;
        while (begin < end) {
            regions.emplace_back(query(handle, begin, ec));
            if (ec)
                return regions;

            begin = regions.back().end();
        }

        return regions;
    }
}

#endif // !VMU_WINDOWS_QUERY_INL
