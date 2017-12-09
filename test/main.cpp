#include <vmu.hpp>
#include <memory>

#include <catch_with_main.hpp>

template<class Address, class Address2>
void check_region(const vmu::basic_region<Address> region, Address2 addr)
{
    REQUIRE(static_cast<bool>(region));
    CHECK(region.protection().readable());
    CHECK(region.protection().writable());
    CHECK(region.size() != 0);
    CHECK(vmu::detail::uintptr_cast(region.begin()) <= vmu::detail::uintptr_cast(addr));
    CHECK(vmu::detail::uintptr_cast(region.end()) >= vmu::detail::uintptr_cast(addr));
}

TEST_CASE("address_cast sanity check")
{
    std::uintptr_t uintptr = 0xdeadbeef;
    void* ptr = vmu::detail::address_cast<void*>(uintptr);
    REQUIRE(memcmp(&uintptr, &ptr, sizeof(void*)) == 0);
}

TEST_CASE("query")
{
    int  i     = 5;
    auto ret   = vmu::query(&i);
    check_region(ret, &i);
}

TEST_CASE("query error code")
{
    std::error_code ec;
    int  i     = 5;
    auto ptr_i = reinterpret_cast<std::uintptr_t>(&i);
    auto ret   = vmu::query(ptr_i, ec);
    check_region(ret, ptr_i);
}

TEST_CASE("query_range")
{
    int  i    = 5;
    auto ptr  = reinterpret_cast<std::uintptr_t>(&i);
    const auto rets = vmu::query_range(ptr, ptr + 16);

    REQUIRE_FALSE(rets.empty());
    for(const auto& ret : rets)
        check_region(ret, ptr);
}

TEST_CASE("query_range error code")
{
    int  i    = 5;
    auto ptr  = reinterpret_cast<std::uintptr_t>(&i);
    std::error_code ec;
    const auto rets = vmu::query_range(&i, &i + 16, ec);

    REQUIRE_FALSE(ec);
    REQUIRE_FALSE(rets.empty());
    for(const auto& ret : rets)
        check_region(ret, ptr);
}

TEST_CASE("protection_guard")
{
    auto random_data = std::make_unique<unsigned char[]>(4096 * 3);
    auto testing_arr = random_data.get();
    memset(testing_arr, 12, 4096 * 3);
    auto result = vmu::query(testing_arr);
    REQUIRE(result);

    {
        const auto new_prot = vmu::access::read;
        CHECK_FALSE(new_prot == result.protection().to_flags());

        vmu::protection_guard pg(testing_arr, new_prot);

        auto new_flags = vmu::query(testing_arr);
        CHECK(new_flags);
        CHECK(new_flags.protection().to_flags() == new_prot);
    }

    auto result2 = vmu::query(testing_arr);
    REQUIRE(result2.shared() == result.shared());
    REQUIRE(result2.begin() == result.begin());
    REQUIRE(result2.protection().to_flags() == result.protection().to_flags());
    REQUIRE(result2.guarded() == result.guarded());
}

TEST_CASE("query_iterator")
{
	const auto total_size = vmu::page_size() * 5;
	auto random_data = std::make_unique<unsigned char[]>(total_size);
	auto begin = random_data.get();
	auto end = begin + total_size;

	vmu::query_iterator end_it(end);
	for (vmu::query_iterator it(begin); it != end_it; ++it) {
		REQUIRE(static_cast<bool>(*it));
		CHECK(it->protection().readable());
		CHECK(it->protection().writable());
		CHECK(it->size() != 0);
	}
}