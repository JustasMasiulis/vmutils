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

#ifndef VMU_MEMORY_ITERATOR_HPP
#define VMU_MEMORY_ITERATOR_HPP

#include <iterator>
#include "query.hpp"

namespace vmu {

    template<class RegionAddress>
    class memory_iterator {
        memory_region<RegionAddress> _region;

    public:
        typedef memory_region             value_type;
        typedef value_type&               reference;
        typedef value_type*               pointer;
        typedef std::ptrdiff_t            difference_type;
        typedef std::forward_iterator_tag iterator_category;

        template<class Address>
        memory_iterator(Address address)
        {
            _region = query(address);
        }

        constexpr reference operator*() const noexcept { return _region; }
        constexpr pointer operator->() const noexcept {return &region; }

        memory_iterator& operator++()
        {
            _region = vmu::query(_region.end());
            return *this;
        }
        memory_iterator operator++(int)
        {
            auto temp = *this;
            _region = vmu::query(_region.end());
            return temp;
        }

        template<class OtherAddress>
        constexpr bool operator==(const memory_iterator<OtherAddress>& other) const noexcept
        {
            return detail::uintptr_cast(other._region.begin())  >= detail::uintptr_cast(_region.begin())
                   && detail::uintptr_cast(other._region.end()) <= detail::uintptr_cast(_region.end());
        }

        template<typename OtherAddress>
        constexpr bool operator!=(const memory_iterator<OtherAddress>& other) const noexcept
        {
            return !(operator==(lhs));
        }
    };

};

#endif // include gaurd
