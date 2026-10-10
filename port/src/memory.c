/* The system-specific part of the runtime: mapping the PS1's memory at its own
 * addresses. A second system is added here and nowhere else. */
#include "port.h"

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>

static int map_range(unsigned address, unsigned size, const char *what, char *err, size_t errsize)
{
    if (!VirtualAlloc((void *)(size_t)address, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
        snprintf(err, errsize, "memory: cannot map %s at 0x%08x (system error %lu)", what, address, (unsigned long)GetLastError());
        return -1;
    }
    return 0;
}

/* Whether [a, a + n) lies in the region [base, base + size), the whole span (64-bit sums). */
static int within(unsigned long long a, unsigned long long n, unsigned long long base, unsigned long long size)
{
    return a >= base && a < base + size && a + n <= base + size;
}

int port_game_span(const void *p, size_t n)
{
    unsigned long long a = (uintptr_t)p;
    /* The live stack runs from this frame's base (the callers' frames are above it) to the base of the stack of the
     * fiber or thread now running, which the system keeps in the thread's block and changes at every fiber switch. */
    unsigned long long low = (uintptr_t)__builtin_frame_address(0);
    unsigned long long high = (uintptr_t)((NT_TIB *)NtCurrentTeb())->StackBase;
    return within(a, n, PORT_RAM_BASE, PORT_RAM_SIZE) || within(a, n, PORT_SCRATCH, PORT_SCRATCH_SIZE) || (low < high && within(a, n, low, high - low));
}

int port_map(char *err, size_t errsize)
{
    if (map_range(PORT_RAM_BASE, PORT_RAM_SIZE, "RAM", err, errsize) != 0) return -1;
    if (map_range(PORT_SCRATCH, PORT_SCRATCH_ALLOC, "scratchpad", err, errsize) != 0) {
        port_unmap();
        return -1;
    }
    return 0;
}

void port_unmap(void)
{
    VirtualFree((void *)(size_t)PORT_RAM_BASE, 0, MEM_RELEASE);
    VirtualFree((void *)(size_t)PORT_SCRATCH, 0, MEM_RELEASE);
}

#else

int port_game_span(const void *p, size_t n)
{
    /* The stack's limits are not read for this system yet: only the two mapped regions. */
    unsigned long long a = (uintptr_t)p;
    return (a >= PORT_RAM_BASE && a < PORT_RAM_BASE + (unsigned long long)PORT_RAM_SIZE && a + n <= PORT_RAM_BASE + (unsigned long long)PORT_RAM_SIZE) ||
           (a >= PORT_SCRATCH && a < PORT_SCRATCH + (unsigned long long)PORT_SCRATCH_SIZE && a + n <= PORT_SCRATCH + (unsigned long long)PORT_SCRATCH_SIZE);
}

int port_map(char *err, size_t errsize)
{
    snprintf(err, errsize, "memory: mapping the PS1's address space is not written for this system yet");
    return -1;
}

void port_unmap(void)
{
}

#endif
