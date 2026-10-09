/* The system-specific part of the runtime: mapping the PS1's memory at its own
 * addresses. A second system is added here and nowhere else. */
#include "port.h"

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

int port_map(char *err, size_t errsize)
{
    snprintf(err, errsize, "memory: mapping the PS1's address space is not written for this system yet");
    return -1;
}

void port_unmap(void)
{
}

#endif
