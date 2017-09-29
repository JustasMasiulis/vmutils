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

#ifndef VMI_WINDOWS_ERROR_HPP
#define VMI_WINDOWS_ERROR_HPP

#include <system_error>
#include "definitions.hpp"

namespace vmu { namespace detail {

    inline std::error_code get_last_error() noexcept
    {
        return std::error_code(static_cast<int>(GetLastError()), std::system_category());
    }

    inline void throw_last_error(const char* msg)
    {
        throw std::system_error(get_last_error(), msg);
    }

}}

#endif // include guard
