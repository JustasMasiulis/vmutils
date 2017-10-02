#include <catch_with_main.hpp>
#include <vmu.hpp>

TEST_CASE("query")
{
    int  i     = 5;
    auto ptr_i = reinterpret_cast<std::uintptr_t>(&i);
    auto ret   = vmu::query(ptr_i);

    REQUIRE(ret);
    CHECK(ret.prot.readable());
    CHECK(ret.prot.writable());
    CHECK(ret.base_address <= ptr_i);
    CHECK(ret.size != 0);
    CHECK(ret.base_address + ret.size >= ptr_i);
    CHECK(ret.end() == ret.base_address + ret.size);
}

TEST_CASE("query_range")
{
    int  i     = 5;
    auto ptr_i = reinterpret_cast<std::uintptr_t>(&i);
    auto rets  = vmu::query_range(ptr_i, ptr_i + 0xff);

    REQUIRE_FALSE(rets.empty());
    for(auto& ret : rets) {
        CHECK(ret.prot.readable());
        CHECK(ret.prot.writable());
        CHECK(ret.base_address <= ptr_i);
        CHECK(ret.size != 0);
        CHECK(ret.base_address + ret.size >= ptr_i);
        CHECK(ret.end() == ret.base_address + ret.size);
    }
}