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

#ifndef VMU_PROTECTION_GUARD_HPP
#define VMU_PROTECTION_GUARD_HPP

#include "query.hpp"
#include "protect.hpp"
#include "address_concept.hpp"

namespace vmu {

    struct adopt_protection_t { explicit adopt_protection_t() = default; };

    /// \brief Provides a RAII based page protection
    class protection_guard {
        struct old_prot_storage {
            std::uintptr_t      begin;
            std::uintptr_t      end;
            native_protection_t prot;
        };

        std::vector<old_prot_storage> _old;
    public:

        /// \brief Restores the memory protection to its state before the protection_guard constructor was called.
		/// \throws Does not throw.
		/// \note May not actually restore memory protection back to its original state.
		///       In critical code sections prefer to manually call restore member function.
		///       This behaviour may be overriden by defining a VMU_HANDLE_PG_DESTRUCTOR_FAILURE macro.
        ~protection_guard()
        {
            std::error_code ec;
			for (const auto& ele : _old) {
				protect(ele.begin, ele.end, ele.prot, ec);
#ifdef VMU_HANDLE_PG_DESTRUCTOR_FAILURE
				if (ec)
					VMU_HANDLE_PG_DESTRUCTOR_FAILURE(ec);
#endif
			}
        }

		/// \brief Changes protection of a single page to given protection.
		/// \param address The address that is situated in page whose protection is to be changed.
		/// \param new_protection The new protection to be applied to the page.
        template<class Address>
        protection_guard(Address address, protection_t new_protection)
        {
            const auto fixed_address = detail::uintptr_cast(address) & ~(page_size() - 1);
            const auto old = query<std::uintptr_t>(address);
            _old.reserve(1);
            _old.push_back({fixed_address, fixed_address + 1, old.protection().native()});
            protect(address, new_protection);
        }

		/// \brief Changes protection of pages between range [begin; end) to given protection.
		/// \param begin The beginning of the memory range to apply new protection to.
		/// \param end One pas the end of the memory range to apply new protection to.
		/// \param new_protection The new protection to be applied to the pages.
        template<class Address>
        protection_guard(Address begin, Address end, protection_t new_protection)
        {
            const auto regions = query_range<std::uintptr_t>(begin, end);
            _old.reserve(regions.size());
            for (auto& region : regions)
                if (region)
                    _old.push_back({region.begin(), region.end(), region.protection().native()});

            protect(begin, end, new_protection);
        }

		/// \brief Changes protection of pages that the given range crosses to given protection.
		/// \param range The memory range whose protection is to be changed.
		/// \param new_protection The new protection to be applied to the pages.
        template<class Range, class = typename std::enable_if<!is_address<Range>::value>::type>
        protection_guard(const Range& range, protection_t new_protection)
                : protection_guard(range.begin(), range.end(), new_protection) {}

		/// \brief Changes protection of a single page to given protection when the protection_guard object is destructed.
		/// \param range The memory range whose protection is to be changed.
		/// \param new_protection The new protection to be applied to the pages.
		template<class Address>
		protection_guard(Address address, protection_t restore_to, adopt_protection_t)
		{
			const auto fixed_address = detail::uintptr_cast(address) & -page_size();
			_old.reserve(1);
			_old.push_back({ fixed_address, fixed_address + 1, restore_to });
		}

		/// \brief Changes the protection of pages in memory range [begin; end) upon destruction of protection_guard object
		/// \param range The memory range whose protection is to be changed.
		/// \param new_protection The new protection to be applied to the pages.
        template<class Address>
        protection_guard(Address begin
                         , Address end
                         , protection_t restore_to
                         , adopt_protection_t)
        {
            _old.reserve(1);
            _old.emplace_back(detail::address_cast_unchecked<std::uintptr_t>(begin)
                              , detail::address_cast<std::uintptr_t>(end)
                              , restore_to);
        }

        template<class Range, class Address>
        protection_guard(const Range& r, protection_t prot, adopt_protection_t)
                : protection_guard(r.begin(), r.end(), prot, adopt_protection_t{}) {}


        /// \brief not copy constructible
        protection_guard(const protection_guard&) = delete;
        /// \brief not copy assignable
        protection_guard& operator=(const protection_guard&) = delete;

		/// \brief Restores the memory protection back to its previous state.
        void restore()
        {
            while (!_old.empty()) {
                const auto& back = _old.back();
                protect(back.begin, back.end, back.prot);
                _old.pop_back();
            }
        }
        void restore(std::error_code& ec) noexcept
        {
            while (!_old.empty()) {
                const auto& back = _old.back();
                protect(back.begin, back.end, back.prot, ec);
                if (ec)
                    return;

                _old.pop_back();
            }
        }

		/// \brief Releases protections to restore.
		/// \throws Should not throw due to the fact that shrink_to_fit is called on empty vector.
		void release()
		{
			_old.clear();
			_old.shrink_to_fit();
		}
    };

}

#endif // include guard
