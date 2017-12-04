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

#ifndef VMU_WINDOWS_PROTECTION_INL
#define VMU_WINDOWS_PROTECTION_INL

#include "../../protection.hpp"
#include <stdexcept>
#include <string>

namespace vmu {

	namespace detail {

		enum page_protection : native_protection_t {
			noaccess = 0x01,
			readonly = 0x02,
			readwrite = 0x04,
			writecopy = 0x08,
			execute = 0x10,
			execute_read = 0x20,
			execute_readwrite = 0x40,
			execute_writecopy = 0x80,

			guard = 0x100,
			nocache = 0x200,
			writecombine = 0x400,
		};

	}

    constexpr inline native_protection_t to_native(access flags)
    {
        switch (flags)
        {
        case access::none:
            return detail::noaccess;
        case access::read:
            return detail::readonly;
        case access::write: // no writeonly
            return detail::readwrite;
        case access::exec:
            return detail::execute;
        case access::read_write:
            return detail::readwrite;
        case access::read | access::exec:
            return detail::execute_read;
        case access::write_exec:
		case access::read_write_exec:
            return detail::execute_readwrite;
        default:
            throw std::logic_error("unknown protection access combination");
        }
    }

    constexpr inline access from_native(native_protection_t flags)
    {
        switch (flags & (~(detail::guard | detail::nocache | detail::writecombine)))
        {
		case detail::noaccess:
            return access::none;
		case detail::readonly:
            return access::read;
		case detail::execute:
            return access::exec;
		case detail::readwrite:
		case detail::writecopy:
            return access::read_write;
		case detail::execute_read:
            return access::read_exec;
		case detail::execute_readwrite:
		case detail::execute_writecopy:
            return access::read_write_exec;
        default:
            throw std::range_error("unknown protection constant: " + std::to_string(flags));
        }
    }

    constexpr bool protection_t::accessible() const noexcept
    {
        return !(_native & detail::noaccess);
    }

    constexpr bool protection_t::readable() const noexcept
    {
        return accessible()
            && ((_native & detail::readwrite)
                || (_native & detail::execute_read)
                || (_native & detail::execute_readwrite)
                || (_native & detail::readonly)
                || (_native & detail::writecopy)
                || (_native & detail::execute_writecopy));
    }

    constexpr bool protection_t::writable() const noexcept
    {
        return accessible()
            && ((_native & detail::readwrite)
                || (_native & detail::execute_readwrite)
                || (_native & detail::execute_writecopy)
                || (_native & detail::writecopy));
    }

    constexpr bool protection_t::executable() const noexcept
    {
        return accessible()
            && ((_native & detail::execute_read)
                || (_native & detail::execute)
                || (_native & detail::execute_readwrite)
                || (_native & detail::execute_writecopy));
    }

} // namespace vmu

#endif // include guard
