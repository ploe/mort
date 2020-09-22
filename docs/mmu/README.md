# What is the MMU?

The Memory Management Unit (MMU) is the module that handles reading and writing to memory addresses.

It is composed of smaller modules that handle the behaviour for the different memory ranges.

It also keeps a local dictionary of labels/symbols parsed from Symfile output by RGBDS.

The smaller units that represent the memory ranges are:

* [rom_t](rom_t.md)
* wram\_t

The Symfile dictionary is implemented in:
* symfile\_t

# Interface

`Mmu_t` is the type. It is a container for other subtypes that represent different memory ranges within the system.

```c
typedef struct Mmu_t {
  /* Mmu_t is a container for the various other memory range containers */
  Rom_t *rom;
  Symfile_t *symfile;
  Wram_t wram;
} Mmu_t;
```

# Methods

## Mmu\_t *NewMmu(const char *path)

Creates a new `Mmu_t` object. `path` is the path to a rom file to create a new `Rom_t` object from.

Returns `NULL` if allocation fails.