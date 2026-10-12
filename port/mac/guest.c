/* The macOS build's side of guest.h (../src/guest.h): the runtime's way to reach the game's memory and code through the machine. */
#include "../src/guest.h"
#include "vm.h"

#include <stdarg.h>
#include <string.h>

void *port_gs(const void *guest, size_t size)
{
    return vm_g((uint32_t)(uintptr_t)guest, size);
}

unsigned port_a(const void *host)
{
    return vm_addr_of(host);
}

uint32_t port_guest_call(port_fn f, unsigned nargs, ...)
{
    uint64_t args[8], result = 0;
    unsigned i;
    va_list ap;

    if (nargs > 8) nargs = 8;
    va_start(ap, nargs);
    for (i = 0; i < nargs; i++) args[i] = va_arg(ap, uint32_t);
    va_end(ap);
    if (vm_call(f, args, nargs, &result) != 0) return 0;   /* the fault hook has run; if it returned, the call gave nothing */
    return (uint32_t)result;
}

#define CELLS 64

void *port_cell(const char *key, size_t size)
{
    static struct { const char *key; uint32_t address; } cells[CELLS];
    unsigned i;
    for (i = 0; i < CELLS && cells[i].key; i++)
        if (strcmp(cells[i].key, key) == 0) return (void *)(uintptr_t)cells[i].address;
    if (i == CELLS) return NULL;
    cells[i].address = vm_alloc(size, 16);
    if (!cells[i].address) return NULL;
    cells[i].key = key;
    return (void *)(uintptr_t)cells[i].address;
}
