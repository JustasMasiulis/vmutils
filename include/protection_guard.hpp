#ifndef VMU_PROTECTION_GUARD_HPP
#define VMU_PROTECTION_GUARD_HPP

#include <vector>
#include <cstdint>
#include "query.hpp"

namespace vmu {

    class protection_guard
    {
        struct old_prot_storage
        {
            std::uintptr_t      begin;
            std::uintptr_t      end;
            native_protection_t prot;
        };

        std::vector<old_prot_storage> _old;
    public:
        protection_guard(std::uintptr_t begin, std::uintptr_t end, protection::storage prot)
        {
            auto regions = query(begin, end);
            for (auto& region : regions) {
                if (!region)
                    throw std::runtime_error("attempt to protect free memory");

                _old.emplace_back(region.begin, region.end, region.prot);
            }
        }

        template<typename Range>
        protection_guard(const Range& r, protection::storage prot);

        /// \brief Restores the memory protection to its previous state
        ~protection_guard()
        {}

        /// \brief not copy constructible
        protection_guard(const protection_guard&) = delete;
        /// \brief not copy assignable
        protection_guard& operator=(const protection_guard&) = delete;

        void restore();
        void restore(std::error_code& ec);
    };

}

#endif // !VMU_PROTECTION_GUARD_HPP
