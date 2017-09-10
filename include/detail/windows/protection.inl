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
#include "winapi.hpp"
#include <stdexcept>
#include <string>

namespace vmu { namespace protection {

    constexpr inline native_protection_t to_native(flags flags)
    {
        switch (flags)
        {
        case flags::none:
            return PAGE_NOACCESS;
        case flags::read:
            return PAGE_READONLY;
        case flags::write: // no writeonly
            return PAGE_READWRITE;
        case flags::exec:
            return PAGE_EXECUTE;
        case flags::read | flags::write:
            return PAGE_READWRITE;
        case flags::read | flags::exec:
            return PAGE_EXECUTE;
        case flags::write | flags::exec:
        case flags::read | flags::write | flags::exec:
            return PAGE_EXECUTE_READWRITE;
        default:
            throw std::logic_error("unknow protection flags combination");
        }
    }

    constexpr inline flags from_native(native_protection_t flags)
    {
        switch (flags & (~(PAGE_GUARD | PAGE_NOCACHE | PAGE_WRITECOMBINE)))
        {
        case PAGE_NOACCESS:
            return flags::none;
        case PAGE_READONLY:
            return flags::read;
        case PAGE_EXECUTE:
            return flags::exec;
        case PAGE_READWRITE: case PAGE_WRITECOPY:
            return (flags::read | flags::write);
        case PAGE_EXECUTE_READ:
            return (flags::read | flags::exec);
        case PAGE_EXECUTE_READWRITE: case PAGE_EXECUTE_WRITECOPY:
            return (flags::read | flags::write | flags::exec);
        default:
            throw std::range_error("unknown protection constant: " + std::to_string(_native));
        }
    }

    constexpr bool storage::accessible() const noexcept
    {
        return !(_native & PAGE_NOACCESS);
    }

    constexpr bool storage::readable() const noexcept
    {
        return accessible()
            && ((_native & PAGE_READWRITE)
                || (_native & PAGE_EXECUTE_READ)
                || (_native & PAGE_EXECUTE_READWRITE)
                || (_native & PAGE_READONLY)
                || (_native & PAGE_EXECUTE_WRITECOPY)
                || (_native & PAGE_WRITECOPY));
    }

    constexpr bool storage::writable() const noexcept
    {
        return accessible()
            && ((_native & PAGE_READWRITE)
                || (_native & PAGE_EXECUTE_READWRITE)
                || (_native & PAGE_EXECUTE_WRITECOPY)
                || (_native & PAGE_WRITECOPY));
    }

    constexpr bool storage::executable() const noexcept
    {
        return accessible()
            && ((_native & PAGE_EXECUTE_READ)
                || (_native & PAGE_EXECUTE)
                || (_native & PAGE_EXECUTE_READWRITE)
                || (_native & PAGE_EXECUTE_WRITECOPY));
    }

}} // namespace vmu::protection

#endif // !VMU_WINDOWS_PROTECTION_INL
