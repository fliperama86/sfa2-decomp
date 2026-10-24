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

static int mapped_extra;   /* the extra memory (port_map_extra) is the game's memory too: its data is read through the game's own offsets */

int port_game_span(const void *p, size_t n)
{
    unsigned long long a = (uintptr_t)p;
    /* The live stack runs from this frame's base (the callers' frames are above it) to the base of the stack of the
     * fiber or thread now running, which the system keeps in the thread's block and changes at every fiber switch. */
    unsigned long long low = (uintptr_t)__builtin_frame_address(0);
    unsigned long long high = (uintptr_t)((NT_TIB *)NtCurrentTeb())->StackBase;
    return within(a, n, PORT_RAM_BASE, PORT_RAM_SIZE) || within(a, n, PORT_SCRATCH, PORT_SCRATCH_SIZE) || (mapped_extra && within(a, n, PORT_EXTRA_BASE, PORT_EXTRA_SIZE)) ||
           (low < high && within(a, n, low, high - low));
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

int port_map_extra(char *err, size_t errsize)
{
    if (mapped_extra) return 0;
    if (!VirtualAlloc((void *)(size_t)PORT_EXTRA_BASE, PORT_EXTRA_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) {
        snprintf(err, errsize, "memory: cannot map the extra memory at 0x%08x (system error %lu)", PORT_EXTRA_BASE, (unsigned long)GetLastError());
        return -1;
    }
    mapped_extra = 1;
    return 0;
}

void port_seal_extra(void)
{
    DWORD old;
    if (mapped_extra) VirtualProtect((void *)(size_t)PORT_EXTRA_BASE, PORT_EXTRA_SIZE, PAGE_READONLY, &old);
}

void port_unmap(void)
{
    VirtualFree((void *)(size_t)PORT_RAM_BASE, 0, MEM_RELEASE);
    VirtualFree((void *)(size_t)PORT_SCRATCH, 0, MEM_RELEASE);
    if (mapped_extra) VirtualFree((void *)(size_t)PORT_EXTRA_BASE, 0, MEM_RELEASE);
    mapped_extra = 0;
}

#elif defined(PORT_LINUX)
/* Linux (port/linux/README.md): the RAM is one memory file, mapped at 0x80000000 and its part from 0x10000 up mapped again at
 * 0x10000, the console's copy of RAM (below 0x10000 the system maps nothing, and mirror.c serves those accesses as faults). */
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define LOW_VIEW PORT_MIRROR_LOW   /* the first address a process can map: the copy of RAM is mapped from here */

static int mapped_ram, mapped_low, mapped_scratch, mapped_extra;   /* the extra memory (port_map_extra) is the game's memory too */

/* Within [base, base + size), the whole span (64-bit sums). */
static int within(unsigned long long a, unsigned long long n, unsigned long long base, unsigned long long size)
{
    return a >= base && a < base + size && a + n <= base + size;
}

int port_game_span(const void *p, size_t n)
{
    unsigned long long a = (uintptr_t)p;
    /* The live stack runs from this frame up to the top of the stack of the task that runs now (threads.c keeps it). */
    unsigned long long low = (uintptr_t)__builtin_frame_address(0);
    unsigned long long high = port_stack_top();
    return within(a, n, PORT_RAM_BASE, PORT_RAM_SIZE) || within(a, n, PORT_SCRATCH, PORT_SCRATCH_SIZE) || (mapped_extra && within(a, n, PORT_EXTRA_BASE, PORT_EXTRA_SIZE)) ||
           (low < high && within(a, n, low, high - low));
}

/* Map at exactly `address`, never over a mapping that is there: MAP_FIXED_NOREPLACE (Linux 4.17). A kernel that does not know the flag
 * takes the address as a hint, so the result is compared too. */
static int map_at(unsigned address, unsigned size, int prot, int flags, int fd, unsigned offset, const char *what, char *err, size_t errsize)
{
    void *want = (void *)(uintptr_t)address;
    void *got = mmap(want, size, prot, flags | MAP_FIXED_NOREPLACE, fd, (off_t)offset);
    if (got == MAP_FAILED) {
        snprintf(err, errsize, "memory: cannot map %s at 0x%08x (%s)", what, address, errno == EEXIST ? "the range is taken already" : strerror(errno));
        return -1;
    }
    if (got != want) {
        munmap(got, size);
        snprintf(err, errsize, "memory: cannot map %s at 0x%08x (the system offered another address)", what, address);
        return -1;
    }
    return 0;
}

int port_map(char *err, size_t errsize)
{
    int fd = memfd_create("sfa2-ram", MFD_CLOEXEC);
    if (fd < 0) {
        snprintf(err, errsize, "memory: cannot make the RAM's memory file (%s)", strerror(errno));
        return -1;
    }
    if (ftruncate(fd, PORT_RAM_SIZE) != 0) {
        snprintf(err, errsize, "memory: cannot size the RAM's memory file (%s)", strerror(errno));
        close(fd);
        return -1;
    }
    if (map_at(PORT_RAM_BASE, PORT_RAM_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0, "RAM", err, errsize) != 0) {
        close(fd);
        return -1;
    }
    mapped_ram = 1;
    if (map_at(LOW_VIEW, PORT_RAM_SIZE - LOW_VIEW, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LOW_VIEW, "the copy of RAM", err, errsize) != 0) {
        close(fd);
        port_unmap();
        return -1;
    }
    mapped_low = 1;
    close(fd);   /* the mappings keep the memory */
    if (map_at(PORT_SCRATCH, PORT_SCRATCH_ALLOC, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0, "scratchpad", err, errsize) != 0) {
        port_unmap();
        return -1;
    }
    mapped_scratch = 1;
    return 0;
}

int port_map_extra(char *err, size_t errsize)
{
    if (mapped_extra) return 0;
    if (map_at(PORT_EXTRA_BASE, PORT_EXTRA_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0, "the extra memory", err, errsize) != 0) return -1;
    mapped_extra = 1;
    return 0;
}

void port_seal_extra(void)
{
    if (mapped_extra) mprotect((void *)(uintptr_t)PORT_EXTRA_BASE, PORT_EXTRA_SIZE, PROT_READ);
}

void port_unmap(void)
{
    if (mapped_ram) munmap((void *)(uintptr_t)PORT_RAM_BASE, PORT_RAM_SIZE);
    if (mapped_low) munmap((void *)(uintptr_t)LOW_VIEW, PORT_RAM_SIZE - LOW_VIEW);
    if (mapped_scratch) munmap((void *)(uintptr_t)PORT_SCRATCH, PORT_SCRATCH_ALLOC);
    if (mapped_extra) munmap((void *)(uintptr_t)PORT_EXTRA_BASE, PORT_EXTRA_SIZE);
    mapped_ram = mapped_low = mapped_scratch = mapped_extra = 0;
}

#elif !defined(PORT_GUEST_VM)   /* the macOS build has its own, in port/mac/platform.c */

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

int port_map_extra(char *err, size_t errsize)
{
    snprintf(err, errsize, "memory: the extra memory is not written for this system yet");
    return -1;
}

void port_seal_extra(void)
{
}

#endif
