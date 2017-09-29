#include <catch_with_main.hpp>
#include <vmi.hpp>

TEST_CASE("main")
{
    int i = 5;
    auto ret = vmu::query(reinterpret_cast<std::uintptr_t>(&i));
    REQUIRE(ret.prot.readable());
    REQUIRE(ret.prot.writable());
    REQUIRE(ret);
}