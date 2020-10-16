# What is the MMU?

The Memory Management Unit (MMU) is the module that handles reading and writing to memory addresses.

It is composed of smaller modules that handle the behaviour for the different memory ranges.

The smaller units that represent the memory ranges are:

* [rom_t](rom_t.md)
* [wram_t](wram_t.md)

# Interface

`Memory_t` is the type. It is a container for other subtypes that represent different memory ranges within the system.

```c
typedef struct Memory_t {
  /* Memory_t is a container for the various other memory range containers */
  Rom_t *rom;
  Wram_t wram;
} Memory_t;
```

# Methods

## Memory_t \*DestroyMemory(Memory_t \*memory)

Deallocate `memory` and its submodules. Always returns `NULL`

## Memory\_t \*NewMemory(const char \*romfile)

Creates a new `Memory_t` object using the rom at `romfile`

Returns `NULL` if allocation fails.
