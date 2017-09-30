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

#ifndef VMU_LINUX_QUERY_INL
#define VMU_LINUX_QUERY_INL

#include "../../query.hpp"
#include <fstream>
#include <sys/types.h>
#include <unistd.h>

namespace vmu { namespace detail {

    inline vmu::protection::storage transform_prot(char prot[4]) noexcept
    {
        return {(prot[0] != '-') | ((prot[1] != '-') * 2) | ((prot[2] != '-') * 4)};
    }

    template<typename Ptr>
    inline basic_region<Ptr> query_impl(std::ifstream& maps, Ptr address)
    {
        char             prot[4];
        Ptr              prev_end   = 0;
        Ptr              prev_begin = 0;
        for (std::string str; maps; maps.ignore(INT32_MAX, '\n')) {
            std::getline(maps, str, '-');
            const auto begin_addr = stoull(str, nullptr, 16);

            if (begin_addr <= address) {
                std::getline(maps, str, ' ');
                const auto end_addr = stoull(str, nullptr, 16);

                if (end_addr >= address) {
                    maps.read(prot, 4);
                    return {begin_addr, end_addr - begin_addr, transform_prot(prot), prot[3] != '-', false, true};
                }
                else
                    prev_end = end_addr;
            }
            else
                prev_begin = begin_addr;
        }

        return {prev_end, prev_begin - prev_end, 0, false, false, false};
    }

    template<typename Ptr>
    inline std::vector<basic_region<Ptr>> query_range_impl(std::ifstream& maps, Ptr begin, Ptr end)
    {
        std::vector<remote_region> regions;
        char                       prot[4];
        for (std::string           buf; maps && begin < end; maps.ignore(INT32_MAX, '\n')) {
            std::getline(maps, buf, '-');

            const auto begin_addr = stoull(buf, nullptr, 16);
            if (begin_addr < begin)
                continue;

            std::getline(maps, buf, ' ');

            const auto end_addr = stoull(buf, nullptr, 16);
            begin = end_addr;

            // free memory
            if (!regions.empty() && regions.back().end() != begin_addr)
                regions.emplace_back(regions.back().end()
                                     , begin_addr - regions.back().end()
                                     , 0
                                     , false
                                     , false
                                     , false);

            maps.read(prot, 4);
            regions.emplace_back(begin_addr, end_addr - begin_addr, transform_prot(prot), prot[3] != '-', false, true);
        }

        return regions;
    }

    inline std::ifstream open_maps(int pid)
    {
        std::ifstream maps;
        maps.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        maps.open("/proc/" + std::to_string(pid) + "/maps");

        return maps;
    }

}}

namespace vmu {

    inline local_region query(std::uintptr_t address)
    {
        std::ifstream maps{detail::open_maps(::getpid())};

        return detail::query_impl<std::uintptr_t>(maps, address);
    }
    inline local_region query(std::uintptr_t address, std::error_code& ec)
    {
        std::ifstream maps("/proc/" + std::to_string(::getpid()) + "/maps");
        if (!maps.is_open()) {
            ec = std::make_error_code(std::errc::io_error);
            return {};
        }

        return detail::query_impl<std::uintptr_t>(maps, address);
    }

    inline std::vector<local_region> query_range(std::uintptr_t begin, std::uintptr_t end)
    {
        std::ifstream maps{detail::open_maps(::getpid())};

        return detail::query_range_impl<std::uintptr_t>(maps, begin, end);
    }
    inline std::vector<local_region> query_range(std::uintptr_t begin, std::uintptr_t end, std::error_code& ec)
    {
        std::ifstream maps("/proc/" + std::to_string(::getpid()) + "/maps");
        if (!maps.is_open()) {
            ec = std::make_error_code(std::errc::io_error);
            return {};
        }

        return detail::query_range_impl<std::uintptr_t>(maps, begin, end);
    }


    template<typename Handle>
    inline remote_region query(Handle handle, std::uint64_t address)
    {
        std::ifstream maps{detail::open_maps(handle)};

        return detail::query_impl<std::uint64_t>(maps, address);
    }
    template<typename Handle>
    inline remote_region query(Handle handle, std::uint64_t address, std::error_code& ec)
    {
        std::ifstream maps("/proc/" + std::to_string(static_cast<int>(handle)) + "/maps");
        if (!maps.is_open()) {
            ec = std::make_error_code(std::errc::io_error);
            return {};
        }

        return detail::query_impl<std::uint64_t>(maps, address);
    }

    template<typename Handle>
    inline std::vector<remote_region> query_range(Handle handle, std::uint64_t begin, std::uint64_t end)
    {
        std::ifstream maps{detail::open_maps(handle)};

        return detail::query_range_impl<std::uint64_t>(maps, begin, end);
    }
    template<typename Handle>
    inline std::vector<remote_region>
    query_range(Handle handle, std::uint64_t begin, std::uint64_t end, std::error_code& ec)
    {
        std::ifstream maps("/proc/" + std::to_string(static_cast<int>(handle)) + "/maps");
        if (!maps.is_open()) {
            ec = std::make_error_code(std::errc::io_error);
            return {};
        }

        return detail::query_range_impl<std::uint64_t>(maps, begin, end);
    }
}

#endif // !VMU_LINUX_QUERY_INL
