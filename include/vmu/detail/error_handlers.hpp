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

#ifndef VMU_ERROR_HANDLERS_HPP
#define VMU_ERROR_HANDLERS_HPP

#include <system_error>

// TODO force inline these

namespace vmu { namespace detail {

    struct handle_with_exception {
        constexpr static bool is_noexcept = false;

        [[noreturn]] inline void operator()(int code, const char* msg) const
        {
            throw std::system_error(std::error_code(code, std::system_category()),
                                    msg);
        }
    };

    struct handle_with_ec {
        constexpr static bool is_noexcept = true;

        std::error_code& _ec;

        inline void operator()(int code, const char*) noexcept
        {
            _ec = std::error_code(code, std::system_category());
        }
    };

}} // namespace vmu::detail

#endif // VMU_ERROR_HANDLERS_HPP
