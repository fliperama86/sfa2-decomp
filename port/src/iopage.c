/* The console's I/O page [0x1F801000, 0x1F802000).
 *
 * The page lies inside the 64 KB that memory.c maps for the scratchpad, where it would be plain memory that the game can
 * read and write and nobody notices. At start (port_iopage_init) the page is made no-access, in every build, so every
 * access of the game to it faults. The fault is served by the handler of mirror.c: it decodes the instruction with the
 * mirror layer's decoder and carries it out with its executor on a temporary value, never on the page. Before an
 * instruction that reads, mirror.c calls port_iopage_read for the temporary; before one that writes, port_iopage_check_write
 * (a refusal then changes no register and no state); after it, port_iopage_write with the value.
 *
 * The models are three plain 32-bit cells that read back what was last written (0 at start):
 *   0x1F801014  the sound chip's access timing (SPU_DELAY)
 *   0x1F8010F0  the DMA control register (DPCR)
 *   0x1F8010F4  the DMA interrupt register (DICR)
 * The game's own sound library writes them when it starts. They are cells and no more: nothing reads them for an effect.
 * An access of another width to one of them, an access that covers only part of one, and every other address of the page
 * end the program with a line (mirror.c prints it with the address, the kind, the function and the instruction's bytes;
 * the reason is the text this file gives back).
 *
 * Not here yet: the sound chip (0x1F801C00 to 0x1F801DFF) and DMA (0x1F8010C0 to 0x1F8010C8).
 *
 * With --trace one line at exit: the faults served and the reads and writes of each cell. */
#include "port.h"
#include "mirror.h"

#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define CELLS 3
static const unsigned cell_addr[CELLS] = { 0x1f801014u, 0x1f8010f0u, 0x1f8010f4u };
static unsigned cell_value[CELLS], cell_reads[CELLS], cell_writes[CELLS], faults;

/* The cell at exactly this address, or -1. */
static int cell_at(unsigned addr)
{
    int i;
    for (i = 0; i < CELLS; i++)
        if (cell_addr[i] == addr) return i;
    return -1;
}

static int check(unsigned addr, unsigned size, char *why, size_t n, int *cell)
{
    *cell = cell_at(addr);
    if (*cell < 0) {
        snprintf(why, n, "with an address that no model of the page claims (0x%08x, %u bytes)", addr, size);
        return -1;
    }
    if (size != 4) {
        snprintf(why, n, "with an access of %u bytes on a register that takes 4", size);
        return -1;
    }
    return 0;
}

int port_iopage_check_write(unsigned addr, unsigned size, char *why, size_t whysize)
{
    int c;
    return check(addr, size, why, whysize, &c);
}

int port_iopage_read(unsigned addr, unsigned size, unsigned *value, char *why, size_t whysize)
{
    int c;
    if (check(addr, size, why, whysize, &c) != 0) return -1;
    *value = cell_value[c];
    cell_reads[c]++;
    return 0;
}

int port_iopage_write(unsigned addr, unsigned size, unsigned value, char *why, size_t whysize)
{
    int c;
    if (check(addr, size, why, whysize, &c) != 0) return -1;
    cell_value[c] = value;
    cell_writes[c]++;
    return 0;
}

/* Called by mirror.c once per served fault. */
void port_iopage_served(void)
{
    faults++;
}

static void report(void)
{
    port_trace_line("iopage: %u faults served; 0x%08x %u reads %u writes, 0x%08x %u reads %u writes, 0x%08x %u reads %u writes", faults,
                    cell_addr[0], cell_reads[0], cell_writes[0], cell_addr[1], cell_reads[1], cell_writes[1], cell_addr[2], cell_reads[2], cell_writes[2]);
}

int port_iopage_init(int trace, char *err, size_t errsize)
{
#ifdef _WIN32
    DWORD old;
    if (!VirtualProtect((void *)(size_t)IO_BASE, IO_END - IO_BASE, PAGE_NOACCESS, &old)) {
        snprintf(err, errsize, "iopage: cannot close the I/O page (system error %lu)", (unsigned long)GetLastError());
        return -1;
    }
#else
    (void)err;
    (void)errsize;
#endif
    if (trace) {
        port_trace_line("iopage: the I/O page 0x%08x..0x%08x closed", IO_BASE, IO_END - 1);
        atexit(report);
    }
    return 0;
}
