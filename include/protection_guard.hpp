#ifndef VMU_PROTECTION_GUARD_HPP
#define VMU_PROTECTION_GUARD_HPP

#include "protection.hpp"

namespace vmu {

    class protection_guard
    {
        
    public:
        protection_guard(std::uintptr_t begin, std::uintptr_t end, protection::storage prot);

        template<typename Range>
        protection_guard(const Range& r, protection prot);

    };

}

#endif // !VMU_PROTECTION_GUARD_HPP
