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

    template<class RegionAddress = std::uintptr_t>
    class basic_query_iterator {
        basic_region<RegionAddress> _region;

    public:
        typedef basic_region<RegionAddress> value_type;
        typedef value_type&                 reference;
        typedef const value_type&           const_reference;
        typedef value_type*                 pointer;
        typedef const value_type*           const_pointer;
        typedef std::ptrdiff_t              difference_type;
        typedef std::forward_iterator_tag   iterator_category;

        template<class Address>
        basic_query_iterator(Address address) : _region(query(address))
        {}

        reference       operator*() noexcept { return _region; }
        const_reference operator*() const noexcept { return _region; }
        pointer         operator->() noexcept { return &_region; }
        const_pointer   operator->() const noexcept { return &_region; }

        basic_query_iterator& operator++()
        {
            _region = vmu::query(_region.end());
            return *this;
        }
        basic_query_iterator operator++(int)
        {
            auto temp = *this;
            _region   = vmu::query(_region.end());
            return temp;
        }

        template<class OtherAddress>
        bool operator==(const basic_query_iterator<OtherAddress>& other) const
            noexcept
        {
            return detail::uintptr_cast(other._region.begin()) >=
                       detail::uintptr_cast(_region.begin()) &&
                   detail::uintptr_cast(other._region.end()) <=
                       detail::uintptr_cast(_region.end());
        }

        template<typename OtherAddress>
        bool operator!=(const basic_query_iterator<OtherAddress>& other) const
            noexcept
        {
            return !(operator==(other));
        }
    };

    using query_iterator = basic_query_iterator<>;

}; // namespace vmu

#endif // include gaurd
