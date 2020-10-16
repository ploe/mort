# What is the ROM?

The Read Only Memory (ROM) is the address range that represents the Game Boy program itself. It is the assembled binary that you, the developer, are wanting to test.

The address range is from `0x0000` to `0x7FFF`

# Interface

`Rom_t` is the type.

`data` is the raw binary data read from the file.

`count` is the size of the raw data read from the file.

```c
typedef struct Rom_t {
  /* Rom_t is a container for the ROM data */
  char *data;
  long count;
} Rom_t;
```

# Methods

## Rom_t \*OpenRom(const char \*path)

Creates a new `Rom_t` from the file at `path`

Returns `NULL` if allocation fails.

## Rom_t \*CloseRom(Rom_t \*rom)

Frees all resources associated with `rom`

Always returns `NULL`.
