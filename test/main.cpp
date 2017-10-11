#include <catch_with_main.hpp>
#include <vmu.hpp>

unsigned char testing_arr[8200];

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
    }
}

TEST_CASE("protection_guard")
{
    // needed on linux
    memset(testing_arr, 1, 8200);
    auto result = vmu::query(testing_arr);
    REQUIRE(result);

    // TODO using FlushInstructionCache might be a thing to do
    {
        const auto new_prot = vmu::access::none;
        CHECK_FALSE(new_prot == result.prot.to_flags());

        vmu::protection_guard pg(testing_arr, new_prot);

        auto new_flags = vmu::query(testing_arr);
        REQUIRE(new_flags);
        REQUIRE(new_flags.prot.to_flags() == new_prot);
    }

    auto result2 = vmu::query(testing_arr);
    REQUIRE(result2.shared == result.shared);
    REQUIRE(result2.in_use == result.in_use);
    REQUIRE(result2.base_address == result.base_address);
    REQUIRE(result2.prot.to_flags() == result.prot.to_flags());
    REQUIRE(result2.guarded == result.guarded);
}
