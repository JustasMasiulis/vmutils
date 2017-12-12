# vmutils [![Build Status](https://travis-ci.org/JustasMasiulis/vmutils.svg?branch=master)](https://travis-ci.org/JustasMasiulis/vmutils) [![Build status](https://ci.appveyor.com/api/projects/status/kty003rauanbu8xh?svg=true)](https://ci.appveyor.com/project/JustasMasiulis/vmutils)

cross platform virtual memory utilities library. 

## small example
```cpp
// address can be unsinged integer or pointer
const auto region = vmu::query(address);
if(region && region.protection().accessible()) {
    const auto new_flags = vmu::access::read_write_exec;
    vmu::protection_guard guard(address, new_flags);
    // do something
} // protection will be restored on scope exit
```

***

# quick reference
## `basic_region<Address>`
Provides information about a memory region. Can be obtained using `query` or `query_region`.

| function      | return type                      | explanation                   |
| ------------- |--------------------------------- | ----------------------------- |
| begin         | Address                          | beginning of region           |
| end           | Address                          | one pas the end of region     |
| size          | uint big enough to store Address | size of region                |
| protection    | protection_t                     | the protection of page        |
| guarded       | bool                             | whether the region is guarded |
| shared        | bool                             | whether the region is shared  |
| operator bool | bool                             | whether the region is used    |

## `protection_t`
Wrapper class around protection flags.

| function   | return type         |
| ---------- |-------------------- |
| accessible | bool                |
| readable   | bool                |
| writable   | bool                |
| executable | bool                |
| native     | native_protection_t |
| to_flags   | access              |

## `access` enumeration
- read
- write
- exec
- read_write
- read_exec
- write_exec
- read_write_exec

## region query functions
Arguments in `[]` are not required - overloaded functions are present.
### `basic_region<RegionAddress> query([native_handle_t], Address, [std::error_code&])`
Returns information about a region that the given address is in.
### `std::vector<basic_region<RegionAddress>> query_range([native_handle_t], Address, Address, [std::error_code&])`
Returns information about regions that the given range of addresses are in.

The default for RegionAddress is `std::uintptr_t` or if the first function argument is of type
`native_handle_t` - `std::uint64_t`.

## region protection
### `protection_guard`
RAII based memory region protection and query class that protects a region of memory and 
restores it upon destruction. 

If adopt_protection_t tag type is given constructor performs no
actions and destructor sets the given protection instead.

In critical code it is recommended to manually call `restore` or define `VMU_HANDLE_PG_DESTRUCTOR_FAILURE`
macro that will handle errors in destructor, because by default on error memory regions is left unrestored.

constructor overloads:
- `protection_guard(Address, protection_t)`
- `protection_guard(Address, Address, protection_t)`
- `protection_guard(Range, protection_t)`
- `protection_guard(Address, protection_t, adopt_protection_t)`
- `protection_guard(Address, Address, protection_t, adopt_protection_t)`
- `protection_guard(Range, protection_t, adopt_protection_t)`

| function                    | explanation                                                             |
| -------------               | ----------------------------------------------------------------------- |
| restore([std::error_code&]) | restores protection to its original state                               |
| release                     | "releases" protection so it does not get restored on object destruction |

### `void protect(Address, protection_t, [std::error_code&])`
Changes protection of a single page that the address is in.
### `void protect(Address, Address, protection_t, [std::error_code&])`
Changes protections of pages that range of addresses are in.

## Address types
In library addresses are 32bit or 64bit unsigned integers or pointers. Conversions are applied
automatically. By default every address that can cause an overflow / underflow is checked and an
std::overflow_error is thrown if the target address type cannot represent the source address type.

An example of this would be if you called `vmu::query<std::uint32_t>(std::numeric_limits<std::uint32_t>::max() * 2)`
it's pretty obvious that this regions end will definetly overflow the requested 32 bit integer.
