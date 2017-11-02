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

#ifndef VMU_QUERY_HPP
#define VMU_QUERY_HPP

#include <vector>
#include <system_error>
#include "region.hpp"

namespace vmu {

    /// \brief The native, operating system dependant handle
#if defined(_WIN32)
    using native_handle_t = void*;
#elif defined(__apple__)
    using native_handle_t = ::mach_port_t;
#else
    using native_handle_t = ::pid_t;
#endif

    /// \brief Performs a query over a memory page located at given address.
    /// \tparam RegionAddress The type to use for address in basic_region.
    /// \tparam Address The type of argument address.
    /// \param address The address of page to be queried.
    /// \throw Throws std::system_error on failure or std::overflow_error if
    ///        address overflows.
    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address);

    /// \brief Performs a query over a memory page located at given address.
    /// \tparam RegionAddress The type to use for address in basic_region.
    /// \tparam Address The type of argument address.
    /// \param address The address of page to be queried.
    /// \throw Does not throw if the address does not overflow.
    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address, std::error_code& ec);

    /// \brief Performs a query over memory pages in range [begin; end)
    /// \tparam RegionAddress The type to use for address in basic_region.
    /// \tparam Address The type of arguments begin and end.
    /// \param begin The beginning of range of pages to query.
    /// \param end One pas the end of pages to query.
    /// \throw Throws std::system_error on failure or std::overflow_error if
    ///        begin or end addresses overflow.
    template<class RegionAddress = std::uintptr_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(Address begin, Address end);

    /// \brief Performs a query over memory pages in range [begin; end)
    /// \tparam RegionAddress The type to use for region.
    /// \tparam Address The type of arguments begin and end.
    /// \param begin The beginning of range of pages to query.
    /// \param end One pas the end of pages to query.
    /// \throw Does not throw if the address does not overflow.
    template<class RegionAddress = std::uintptr_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(Address begin, Address end, std::error_code& ec);


    /// \brief Performs a query over a memory page located at given address.
    /// \tparam RegionAddress The type to use for address in basic_region.
    /// \tparam Address The type of argument address.
    /// \param handle Handle to the target process.
    /// \param address The address of page to be queried.
    /// \throw Throws std::system_error on failure or std::overflow_error if
    ///        address overflows.
    template<class RegionAddress = std::uint64_t, class Address>
    inline basic_region<RegionAddress> query(native_handle_t handle, Address address);

    /// \brief Performs a query over a memory page located at given address.
    /// \tparam RegionAddress The type to use for address in basic_region.
    /// \tparam Address The type of argument address.
    /// \param handle Handle to the target process.
    /// \param address The address of page to be queried.
    /// \throw Does not throw if the address does not overflow.
    template<class RegionAddress = std::uint64_t, class Address>
    inline basic_region<RegionAddress>
    query(native_handle_t handle, Address address, std::error_code& ec);

    /// \brief Performs a query over memory pages in range [begin; end)
    /// \tparam RegionAddress The type to use for address in basic_region.
    /// \tparam Address The type of arguments begin and end.
    /// \param handle Handle to the target process.
    /// \param begin The beginning of range of pages to query.
    /// \param end One pas the end of pages to query.
    /// \throw Throws std::system_error on failure or std::overflow_error if
    ///        begin or end addresses overflow.
    template<class RegionAddress = std::uint64_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end);

    /// \brief Performs a query over memory pages in range [begin; end)
    /// \tparam RegionAddress The type to use for region.
    /// \tparam Address The type of arguments begin and end.
    /// \param handle Handle to the target process.
    /// \param begin The beginning of range of pages to query.
    /// \param end One pas the end of pages to query.
    /// \throw Does not throw if the address does not overflow.
    template<class RegionAddress = std::uint64_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end, std::error_code& ec);

}

#if defined(_WIN32)
    #include "detail/windows/query.inl"
    #include "detail/query_range_using_query.inl"
#elif defined(__APPLE__)
    #include "detail/osx/query.inl"
    #include "detail/query_range_using_query.inl"
#else
    #include "detail/linux/query.inl"
#endif

#endif // include guard
