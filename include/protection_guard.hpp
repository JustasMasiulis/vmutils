#ifndef JM_PROTECTION_GUARD_HPP
#define JM_PROTECTION_GUARD_HPP

#include <cstdint>

namespace jm {

    enum class protection
    {
        none            = 0,
        read            = 1,
        write           = 2,
        read_write      = 3,
        exec            = 4,
        read_exec       = 5,
        write_exec      = 6,
        read_write_exec = 7
    };



    class protection_guard
    {
        
    public:
        template<typename Ptr>
        protection_guard(Ptr begin, Ptr end, protection prot);
    };

}

#endif // !JM_PROTECTION_GUARD_HPP
