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

    constexpr inline native_protection_t to_native(access flags)
    {
        switch (flags)
        {
        case access::none:
            return PAGE_NOACCESS;
        case access::read:
            return PAGE_READONLY;
        case access::write: // no writeonly
            return PAGE_READWRITE;
        case access::exec:
            return PAGE_EXECUTE;
        case access::read | access::write:
            return PAGE_READWRITE;
        case access::read | access::exec:
            return PAGE_EXECUTE;
        case access::write | access::exec:
        case access::read | access::write | access::exec:
            return PAGE_EXECUTE_READWRITE;
        default:
            throw std::logic_error("unknown protection access combination");
        }
    }

    constexpr inline access from_native(native_protection_t flags)
    {
        switch (flags & (~(PAGE_GUARD | PAGE_NOCACHE | PAGE_WRITECOMBINE)))
        {
        case PAGE_NOACCESS:
            return access::none;
        case PAGE_READONLY:
            return access::read;
        case PAGE_EXECUTE:
            return access::exec;
        case PAGE_READWRITE: case PAGE_WRITECOPY:
            return (access::read | access::write);
        case PAGE_EXECUTE_READ:
            return (access::read | access::exec);
        case PAGE_EXECUTE_READWRITE: case PAGE_EXECUTE_WRITECOPY:
            return (access::read | access::write | access::exec);
        default:
            throw std::range_error("unknown protection constant: " + std::to_string(flags));
        }
    }

    constexpr bool protection_t::accessible() const noexcept
    {
        return !(_native & PAGE_NOACCESS);
    }

    constexpr bool protection_t::readable() const noexcept
    {
        return accessible()
            && ((_native & PAGE_READWRITE)
                || (_native & PAGE_EXECUTE_READ)
                || (_native & PAGE_EXECUTE_READWRITE)
                || (_native & PAGE_READONLY)
                || (_native & PAGE_EXECUTE_WRITECOPY)
                || (_native & PAGE_WRITECOPY));
    }

    constexpr bool protection_t::writable() const noexcept
    {
        return accessible()
            && ((_native & PAGE_READWRITE)
                || (_native & PAGE_EXECUTE_READWRITE)
                || (_native & PAGE_EXECUTE_WRITECOPY)
                || (_native & PAGE_WRITECOPY));
    }

    constexpr bool protection_t::executable() const noexcept
    {
        return accessible()
            && ((_native & PAGE_EXECUTE_READ)
                || (_native & PAGE_EXECUTE)
                || (_native & PAGE_EXECUTE_READWRITE)
                || (_native & PAGE_EXECUTE_WRITECOPY));
    }

} // namespace vmu

#endif // include guard
