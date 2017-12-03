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

// windows was not included - need to define all the macros myself
#ifndef _WINDOWS_

// dont want to leak all these definitions so a namespace macro is needed
#define VMU_PROT_MACRO_NAMESPACE detail::

namespace vmu { namespace detail {

	constexpr native_protection_t PAGE_NOACCESS          = 0x01;
	constexpr native_protection_t PAGE_READONLY          = 0x02;
	constexpr native_protection_t PAGE_READWRITE         = 0x04;
	constexpr native_protection_t PAGE_WRITECOPY         = 0x08;
	constexpr native_protection_t PAGE_EXECUTE           = 0x10;
	constexpr native_protection_t PAGE_EXECUTE_READ      = 0x20;
	constexpr native_protection_t PAGE_EXECUTE_READWRITE = 0x40;
	constexpr native_protection_t PAGE_EXECUTE_WRITECOPY = 0x80;

	constexpr native_protection_t PAGE_GUARD        = 0x100;
	constexpr native_protection_t PAGE_NOCACHE      = 0x200;
	constexpr native_protection_t PAGE_WRITECOMBINE = 0x400;

}}

#else

#define VMU_PROT_MACRO_NAMESPACE

#endif

namespace vmu {

    constexpr inline native_protection_t to_native(access flags)
    {
        switch (flags)
        {
        case access::none:
            return VMU_PROT_MACRO_NAMESPACE PAGE_NOACCESS;
        case access::read:
            return VMU_PROT_MACRO_NAMESPACE PAGE_READONLY;
        case access::write: // no writeonly
            return VMU_PROT_MACRO_NAMESPACE PAGE_READWRITE;
        case access::exec:
            return VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE;
        case access::read_write:
            return VMU_PROT_MACRO_NAMESPACE PAGE_READWRITE;
        case access::read | access::exec:
            return VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READ;
        case access::write_exec:
		case access::read_write_exec:
            return VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READWRITE;
        default:
            throw std::logic_error("unknown protection access combination");
        }
    }

    constexpr inline access from_native(native_protection_t flags)
    {
        switch (flags & (~(VMU_PROT_MACRO_NAMESPACE PAGE_GUARD | VMU_PROT_MACRO_NAMESPACE PAGE_NOCACHE | VMU_PROT_MACRO_NAMESPACE PAGE_WRITECOMBINE)))
        {
        case VMU_PROT_MACRO_NAMESPACE PAGE_NOACCESS:
            return access::none;
        case VMU_PROT_MACRO_NAMESPACE PAGE_READONLY:
            return access::read;
        case VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE:
            return access::exec;
        case VMU_PROT_MACRO_NAMESPACE PAGE_READWRITE:
		case VMU_PROT_MACRO_NAMESPACE PAGE_WRITECOPY:
            return access::read_write;
        case VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READ:
            return access::read_exec;
        case VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READWRITE:
		case VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_WRITECOPY:
            return access::read_write_exec;
        default:
            throw std::range_error("unknown protection constant: " + std::to_string(flags));
        }
    }

    constexpr bool protection_t::accessible() const noexcept
    {
        return !(_native & VMU_PROT_MACRO_NAMESPACE PAGE_NOACCESS);
    }

    constexpr bool protection_t::readable() const noexcept
    {
        return accessible()
            && ((_native & VMU_PROT_MACRO_NAMESPACE PAGE_READWRITE)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READ)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READWRITE)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_READONLY)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_WRITECOPY)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_WRITECOPY));
    }

    constexpr bool protection_t::writable() const noexcept
    {
        return accessible()
            && ((_native & VMU_PROT_MACRO_NAMESPACE PAGE_READWRITE)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READWRITE)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_WRITECOPY)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_WRITECOPY));
    }

    constexpr bool protection_t::executable() const noexcept
    {
        return accessible()
            && ((_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READ)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_READWRITE)
                || (_native & VMU_PROT_MACRO_NAMESPACE PAGE_EXECUTE_WRITECOPY));
    }

} // namespace vmu

#endif // include guard
