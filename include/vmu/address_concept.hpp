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

#include <type_traits>
#include <cstdint>

#ifndef VMU_ADDRESS_CONCEPT_HPP
#define VMU_ADDRESS_CONCEPT_HPP

namespace vmu {

    // An address is a pointer of any type or a 32 bit or 64 bit unsigned integer
    template<class T>
    struct is_address {
        constexpr static bool value = std::is_pointer<T>::value
                                      || ((sizeof(T) == 4 || sizeof(T) == 8)
                                          && std::is_integral<T>::value
                                          && std::is_unsigned<T>::value);
    };

}

#endif // include guard
