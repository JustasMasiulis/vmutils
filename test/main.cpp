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

TEST_CASE("query error code")
{
    std::error_code ec;
    int  i     = 5;
    auto ptr_i = reinterpret_cast<std::uintptr_t>(&i);
    auto ret   = vmu::query(ptr_i, ec);

    REQUIRE_FALSE(ec);
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
    int  i    = 5;
    auto ptr  = reinterpret_cast<std::uintptr_t>(&i);
    const auto rets = vmu::query_range(&i, &i + 16);

    REQUIRE_FALSE(rets.empty());
    for(const auto& ret : rets) {
        CHECK(ret);
        CHECK(ret.prot.readable());
        CHECK(ret.prot.writable());
        CHECK(ret.base_address <= ptr);
        CHECK(ret.size != 0);
        CHECK(ret.base_address + ret.size >= ptr);
        CHECK(ret.end() == ret.base_address + ret.size);
    }
}

TEST_CASE("query_range error code")
{
    int  i    = 5;
    auto ptr  = reinterpret_cast<std::uintptr_t>(&i);
    std::error_code ec;
    const auto rets = vmu::query_range(&i, &i + 16, ec);

    REQUIRE_FALSE(ec);
    REQUIRE_FALSE(rets.empty());
    for(const auto& ret : rets) {
        CHECK(ret);
        CHECK(ret.prot.readable());
        CHECK(ret.prot.writable());
        CHECK(ret.base_address <= ptr);
        CHECK(ret.size != 0);
        CHECK(ret.base_address + ret.size >= ptr);
        CHECK(ret.end() == ret.base_address + ret.size);
    }
}