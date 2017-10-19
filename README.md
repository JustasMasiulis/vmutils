# vmutils [![Build Status](https://travis-ci.com/JustasMasiulis/vmutils.svg?branch=master)](https://travis-ci.com/JustasMasiulis/vmutils)

WIP virtual memory utilities. Not production or general usage ready.

## small example
```cpp
auto region = vmu::query(address);
if(region && region.protection().accessible()) {
    const auto new_flags = vmu::access::read | vmu::acess::write | vmu::access::execute;
    vmu::protection_guard(address, new_flags);
    // do something
} // protection will be restored on scope exit
```
