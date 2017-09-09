#ifndef VMU_REGION_HPP
#define VMU_REGION_HPP

#include "protection.hpp"

namespace vmu {

    template<typename Ptr>
    struct basic_region
    {
        Ptr                 begin;
        Ptr                 end;
        protection::storage prot;
        bool                shared;

        Ptr size() const noexcept { return end - begin; }
        bool guarded() const noexcept { return }
    };


    using local_region  = basic_region<std::uintptr_t>;
    using remote_region = basic_region<std::uint64_t>;

}

#endif // !VMU_REGION_HPP