#include <catch_with_main.hpp>
#include <vmu.hpp>

TEST_CASE("main")
{
    int  i     = 5;
    auto ptr_i = reinterpret_cast<std::uintptr_t>(&i);
    auto ret   = vmu::query(ptr_i);

    REQUIRE(ret.prot.readable());
    REQUIRE(ret.prot.writable());
    REQUIRE(ret.base_address >= ptr_i);
    REQUIRE(ret.size != 0);
    REQUIRE(ret.base_address + ret.size <= ptr_i);
    REQUIRE(ret.end() == ret.base_address + ret.size);
    REQUIRE(ret);
}