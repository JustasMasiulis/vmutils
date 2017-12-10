# vmutils [![Build Status](https://travis-ci.org/JustasMasiulis/vmutils.svg?branch=master)](https://travis-ci.org/JustasMasiulis/vmutils) [![Build status](https://ci.appveyor.com/api/projects/status/kty003rauanbu8xh?svg=true)](https://ci.appveyor.com/project/JustasMasiulis/vmutils)

cross platform virtual memory utilities library. 

## small example
```cpp
// also possible to query a range using vmu::query_range(begin, end)
const auto region = vmu::query(address);
if(region && region.protection().accessible()) {
    // vmu::access::read_write_exec is also valid
    const auto new_flags = vmu::access::read | vmu::acess::write | vmu::access::execute;
    vmu::protection_guard(address, new_flags);
    // do something
} // protection will be restored on scope exit
```

***

# quick reference
## `basic_region<AddressType>`
Provides information about a memory region. Can be obtained using `query` or `query_region`.

| function      | type                                 | returns                       |
| ------------- |------------------------------------- | ----------------------------- |
| begin         | AddressType                          | beginning of region           |
| end           | AddressType                          | one pas the end of region     |
| size          | uint big enough to store AddressType | size of region                |
| protection    | protection_t                         | the protection of page        |
| guarded       | bool                                 | whether the region is guarded |
| shared        | bool                                 | whether the region is shared  |
| operator bool | bool                                 | whether the region is used    |

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
### `basic_region<RegionAddressType> query([native_handle_t], AddressType, [std::error_code&])`
Returns information about a region that the given address is in.
### `std::vector<basic_region<RegionAddressType>> query_range([native_handle_t], AddressType, AddressType, [std::error_code&])`
Returns information about regions that the given range of addresses are in.

The default for RegionAddressType is `std::uintptr_t` or if the first function argument is of type
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
