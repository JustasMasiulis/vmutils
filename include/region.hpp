#ifndef VMU_REGION_HPP
#define VMU_REGION_HPP

#include <cstdint>
#include "protection.hpp"

namespace vmu {

    template<typename Ptr>
    struct basic_region
    {
        Ptr                 begin;
        Ptr                 end;
        protection::storage prot;
        bool                shared;

        constexpr Ptr size() const noexcept { return end - begin; }
        constexpr bool guarded() const noexcept 
        {
#ifdef VMU_WINDOWS
            return (prot.native() & PAGE_GUARD);
#else
            return false;
#endif
        }
    };


    using local_region  = basic_region<std::uintptr_t>;
    using remote_region = basic_region<std::uint64_t>;

}

#endif // !VMU_REGION_HPP