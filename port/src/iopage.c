/* The console's I/O page [0x1F801000, 0x1F802000).
 *
 * The page lies inside the 64 KB that memory.c maps for the scratchpad, where it would be plain memory that the game can
 * read and write and nobody notices. At start (port_iopage_init) the page is made no-access, in every build, so every
 * access of the game to it faults. The fault is served by the handler of mirror.c: it decodes the instruction with the
 * mirror layer's decoder and carries it out with its executor on a temporary value, never on the page. Before an
 * instruction that reads, mirror.c calls port_iopage_read for the temporary; before one that writes, port_iopage_check_write
 * (a refusal then changes no register and no state); after it, port_iopage_write with the value.
 *
 * The models:
 *   0x1F801C00..0x1F801DFF  the sound chip, 16-bit accesses at even addresses, through PsyZ's model of it
 *                           (Psyz_SpuRead and Psyz_SpuWrite with the offset from 0x1F801C00). Only in a build with PsyZ
 *                           (PORT_HAVE_PSYZ); without it the addresses have no model and stop.
 *   0x1F8010C0 0x1F8010C4 0x1F8010C8  DMA channel 4 (address, block control, control), 32 bits, each reads back what was
 *                           written, but bit 24 of the control reads clear after a transfer. A write to the control with
 *                           bit 24 set is a transfer, done at once (see "The transfer"). Only in a build with PsyZ.
 *   0x1F801014 0x1F8010F0 0x1F8010F4  plain 32-bit cells that read back what was last written (0 at start): the chip's
 *                           access timing, the DMA control register and the DMA interrupt register. They are cells and no
 *                           more: nothing reads them for an effect.
 * An access of another width to a register, an access that covers only part of one, a 16-bit access at an odd address of
 * the chip, and every other address of the page end the program with a line (mirror.c prints it with the address, the kind,
 * the function and the instruction's bytes; the reason is the text this file gives back).
 *
 * The audio lock. Every access to PsyZ's model of the chip is made between Psyz_AudioLock and Psyz_AudioUnlock. They are
 * harmless while no audio layer runs: in PsyZ's src/platform/sdl3_audio.c the mutex they take is created by Psyz_AudioInit
 * (line 49) and both routines do nothing while it does not exist (lines 90 to 99, "if (mutex)"). This program opens no audio
 * device in this version, so the calls take nothing; a later version that opens one gets the exclusion with no change here.
 * Psyz_SpuInit runs once, at start, before the first access.
 *
 * The transfer. The library's start of a transfer (the sound library's _spu_t in libspu/spu_p2.c) writes the address, the
 * block control (blocks of 16 words, so a multiple of 64 bytes) and the control 0x01000201 (to the chip) or 0x01000200 (from
 * the chip). The model does it at once: (block control >> 16) x (block control & 0xFFFF) x 4 bytes between RAM and the chip's
 * RAM (Psyz_SpuFifoWriteBulk at the chip's transfer address, or Psyz_SpuMemRead there and then the address moved on). What
 * it does not trust: an address whose top byte is not 0x00 or 0x80; a span that does not lie wholly inside the 2 MB of RAM
 * (computed in 64 bits); a block control with a zero half; a span longer than the chip's 512 KB of RAM (PsyZ's bulk routines
 * wrap once and would write past its memory); and a start while the last transfer's completion is still pending (the library
 * never does that: SpuIsTransferCompleted in libspu/s_itc.c waits for the event of the completion, and
 * SpuClearReverbWorkArea in libspu/s_crwa.c calls WaitEvent after each start). Each ends the program with a line. These are
 * checked when the control is written, before the model changes; the instruction itself has been carried out on its
 * temporary by then.
 *
 * The completion. After a transfer a completion is pending: the routine registered with DMACallback(4, routine) is called,
 * on the game's thread and never from the fault handler, and never inside a handler of the game or between
 * EnterCriticalSection and ExitCriticalSection (the vblank's rules, by the same variables of kernel.c). It is taken exactly
 * once, by whichever comes first:
 *   1. the first poll of the game after the transfer's start (port_tick, so TestEvent, WaitEvent, VSync and the others
 *      that tick), with no delay;
 *   2. the timer thread of interrupt.c, which redirects the game's thread as it does for a vblank, not before a due time:
 *      the start plus 13 ticks of the console's clock (33,868,800 a second) per byte (port_iopage_claim_due; the interruption
 *      routine then calls port_iopage_deliver_claimed);
 *   3. the vblank's delivery, before the vblank's own handlers (port_iopage_deliver).
 * THE FIGURE OF 13 TICKS A BYTE IS NOT VERIFIED. It is the top level's memory of an emulator's constant (26 ticks per 16-bit
 * word), written down in the private trial's header of this file; no source of this repository gives it. It sets only how
 * long the timer path waits (about 0.8 ms for a sector of 2,048 bytes); the poll and the vblank have no delay.
 * Why the poll has none and the timer has one: a poll is a call the game makes later, never the instructions that follow the
 * start, so the delay that keeps the completion from coming before the game has gone on does not apply to it, and tying the
 * completion to the game's progress rather than to the machine's speed was what the trial's runs needed. The timer thread
 * is the asynchronous way, which is only for a game that does not poll; there a real-time due time is the stand-in for the
 * console's clock.
 * The routine's address comes from the game and is checked at each call (port_target_check), not when it is registered.
 * With no routine registered a completion is dropped and counted.
 *
 * With --trace, at exit: one line with the faults served and the reads and writes of each cell, and one with the chip's and
 * the DMA's counts and the completions taken and how. */
#include "port.h"
#include "mirror.h"

#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef PORT_HAVE_PSYZ
#include <psyz.h>
#include <psyz/spu.h>
#include <psyz/audio.h>
#endif

#define SPU_BASE  0x1f801c00u
#define SPU_END   0x1f801e00u
#define DMA4_MADR 0x1f8010c0u
#define DMA4_BCR  0x1f8010c4u
#define DMA4_CHCR 0x1f8010c8u
#define CHCR_START (1u << 24)
#define SPU_RAM   (512u * 1024u)   /* the chip's RAM, which PsyZ's bulk routines wrap once */

#define CELLS 3
static const unsigned cell_addr[CELLS] = { 0x1f801014u, 0x1f8010f0u, 0x1f8010f4u };
static unsigned cell_value[CELLS], cell_reads[CELLS], cell_writes[CELLS], faults;

#ifdef PORT_HAVE_PSYZ
static unsigned dma_madr, dma_bcr, dma_chcr;
#endif
static unsigned chip_reads, chip_writes, dma_reads, dma_writes, transfers, bytes_to_chip, bytes_from_chip;

/* The completion. State: 0 none, 1 pending, 2 claimed by the timer thread, which is redirecting the game's thread to deliver
 * it, 3 taken by a poll or by the vblank's delivery and about to be run. */
static void (*dma_callback[7])(void);
static volatile int completion_state;
static unsigned long long completion_due_us;
static unsigned taken[4], dropped;   /* completions taken by how: 1 poll, 2 timer, 3 vblank; dropped for want of a routine */

#ifdef PORT_HAVE_PSYZ
#define LOCK() Psyz_AudioLock()
#define UNLOCK() Psyz_AudioUnlock()
#endif

/* The cell at exactly this address, or -1. */
static int cell_at(unsigned addr)
{
    int i;
    for (i = 0; i < CELLS; i++)
        if (cell_addr[i] == addr) return i;
    return -1;
}

enum { G_NONE, G_CELL, G_DMA, G_SPU };

/* Which model claims the address, and 0 or -1 with the reason. */
static int check(unsigned addr, unsigned size, char *why, size_t n, int *group, int *index)
{
    *group = G_NONE;
    *index = cell_at(addr);
    if (*index >= 0) {
        *group = G_CELL;
        if (size != 4) {
            snprintf(why, n, "with an access of %u bytes on a register that takes 4", size);
            return -1;
        }
        return 0;
    }
#ifdef PORT_HAVE_PSYZ
    if (addr == DMA4_MADR || addr == DMA4_BCR || addr == DMA4_CHCR) {
        *group = G_DMA;
        *index = (int)((addr - DMA4_MADR) / 4);
        if (size != 4) {
            snprintf(why, n, "with an access of %u bytes on a register that takes 4", size);
            return -1;
        }
        return 0;
    }
    if (addr >= SPU_BASE && addr < SPU_END) {
        *group = G_SPU;
        if (size != 2) {
            snprintf(why, n, "with an access of %u bytes on a register that takes 2", size);
            return -1;
        }
        if (addr & 1) {
            snprintf(why, n, "with a 16-bit access at the odd address 0x%08x", addr);
            return -1;
        }
        return 0;
    }
#endif
    snprintf(why, n, "with an address that no model of the page claims (0x%08x, %u bytes)", addr, size);
    return -1;
}

int port_iopage_check_write(unsigned addr, unsigned size, char *why, size_t whysize)
{
    int g, i;
    return check(addr, size, why, whysize, &g, &i);
}

int port_iopage_read(unsigned addr, unsigned size, unsigned *value, char *why, size_t whysize)
{
    int g, i;
    if (check(addr, size, why, whysize, &g, &i) != 0) return -1;
    switch (g) {
#ifdef PORT_HAVE_PSYZ
    case G_SPU:
        LOCK();
        *value = Psyz_SpuRead(addr - SPU_BASE);
        UNLOCK();
        chip_reads++;
        break;
    case G_DMA:
        *value = i == 0 ? dma_madr : i == 1 ? dma_bcr : dma_chcr;
        dma_reads++;
        break;
#endif
    default:
        *value = cell_value[i];
        cell_reads[i]++;
        break;
    }
    return 0;
}

#ifdef PORT_HAVE_PSYZ
/* The start of a transfer: every check first, then the transfer, then the pending completion. -1 with the reason and nothing changed. */
static int dma_start(unsigned control, char *why, size_t n)
{
    unsigned blocks = dma_bcr >> 16, words = dma_bcr & 0xffffu, top = dma_madr >> 24, off = dma_madr & 0x00ffffffu;
    unsigned long long bytes = (unsigned long long)blocks * words * 4ull;
    unsigned char *ram;
    if (blocks == 0 || words == 0) {
        snprintf(why, n, "with a DMA block control 0x%08x that has a zero half", dma_bcr);
        return -1;
    }
    if (top != 0 && top != 0x80) {
        snprintf(why, n, "with a DMA address 0x%08x whose top byte is not 0x00 or 0x80", dma_madr);
        return -1;
    }
    if ((unsigned long long)off + bytes > PORT_RAM_SIZE) {
        snprintf(why, n, "with a DMA of %llu bytes at 0x%08x that does not lie in RAM", bytes, dma_madr);
        return -1;
    }
    if (bytes > SPU_RAM) {
        snprintf(why, n, "with a DMA of %llu bytes, more than the chip's RAM of %u", bytes, SPU_RAM);
        return -1;
    }
    if (completion_state != 0) {
        snprintf(why, n, "with a DMA start while the completion of the last transfer is pending");
        return -1;
    }
    ram = (unsigned char *)(size_t)(PORT_RAM_BASE + off);
    LOCK();
    if (control & 1) {
        Psyz_SpuFifoWriteBulk(ram, (unsigned)bytes);
        bytes_to_chip += (unsigned)bytes;
    } else {
        unsigned at = Psyz_SpuGetTransferAddr();
        Psyz_SpuMemRead(at, ram, (unsigned)bytes);
        Psyz_SpuSetTransferAddr(at + (unsigned)bytes);
        bytes_from_chip += (unsigned)bytes;
    }
    UNLOCK();
    transfers++;
    completion_due_us = port_now_us() + bytes * 13ull * 1000000ull / 33868800ull;
    completion_state = 1;
    return 0;
}
#endif

int port_iopage_write(unsigned addr, unsigned size, unsigned value, char *why, size_t whysize)
{
    int g, i;
    if (check(addr, size, why, whysize, &g, &i) != 0) return -1;
    switch (g) {
#ifdef PORT_HAVE_PSYZ
    case G_SPU:
        LOCK();
        Psyz_SpuWrite(addr - SPU_BASE, (unsigned short)value);
        UNLOCK();
        chip_writes++;
        break;
    case G_DMA:
        dma_writes++;
        if (i == 0) dma_madr = value;
        else if (i == 1) dma_bcr = value;
        else {
            if ((value & CHCR_START) && dma_start(value, why, whysize) != 0) return -1;
            dma_chcr = value & ~CHCR_START;
        }
        break;
#endif
    default:
        cell_value[i] = value;
        cell_writes[i]++;
        break;
    }
    return 0;
}

/* Called by mirror.c once per served fault. */
void port_iopage_served(void)
{
    faults++;
}

/* ---- DMACallback and the completion ---- */

/* PSY-Q's DMACallback(dma, func) (libetc intr.c, the game's library at 0x8015f020): sets the routine called when channel
 * `dma` (0 to 6) ends a transfer, and returns the previous one, as InterruptCallback here. Only channel 4 is ever raised. The
 * library registers its routine with DMACallback(4, _spu_FiDMA) in _SpuInit and removes it with DMACallback(4, 0) in SpuQuit
 * (libspu/s_dcb.c's _SpuDataCallback, from s_ini.c and s_q.c). A channel outside 0 to 6 changes nothing and returns 0. */
void *port_h_DMACallback(int dma, void (*func)(void))
{
    void (*previous)(void);
    if (dma < 0 || dma > 6) return NULL;
    previous = dma_callback[dma];
    dma_callback[dma] = func;
    return (void *)previous;
}

/* The completion is taken: the state is cleared first (the routine may start a transfer), then the routine is called, its
 * address checked now. The caller has raised port_handler_depth, so a poll inside the routine does nothing. */
static void run_completion(int how)
{
    completion_state = 0;
    taken[how]++;
    if (dma_callback[4]) {
        port_target_check("dma callback", (const void *)dma_callback[4]);
        dma_callback[4]();
    } else {
        dropped++;
    }
}

/* A poll of the game's thread (kernel.c has checked the rules: not inside a handler, interrupts enabled): the pending
 * completion is taken at once, with no due time. */
void port_iopage_poll(void)
{
    if (completion_state == 1 && __sync_bool_compare_and_swap(&completion_state, 1, 3)) {
        port_handler_depth++;
        run_completion(1);
        port_handler_depth--;
    }
}

/* The vblank's delivery (kernel.c, port_handler_depth already raised), before the vblank's handlers. */
void port_iopage_deliver(void)
{
    if (completion_state == 1 && __sync_bool_compare_and_swap(&completion_state, 1, 3)) run_completion(3);
}

/* The timer thread's claim, with the game's thread suspended where an interruption is allowed: the completion is pending and
 * its due time has passed. 1: claimed, taken once; the interruption routine then calls port_iopage_deliver_claimed. The timer
 * thread only reads the clock and sets the state here. */
int port_iopage_claim_due(void)
{
    return completion_state == 1 && port_now_us() >= completion_due_us && __sync_bool_compare_and_swap(&completion_state, 1, 2);
}

/* The interruption routine's work, on the game's thread: deliver what the timer claimed. */
void port_iopage_deliver_claimed(void)
{
    if (completion_state == 2) {
        port_handler_depth++;
        run_completion(2);
        port_handler_depth--;
    }
}

static void report(void)
{
    port_trace_line("iopage: %u faults served; 0x%08x %u reads %u writes, 0x%08x %u reads %u writes, 0x%08x %u reads %u writes", faults,
                    cell_addr[0], cell_reads[0], cell_writes[0], cell_addr[1], cell_reads[1], cell_writes[1], cell_addr[2], cell_reads[2], cell_writes[2]);
    port_trace_line("iopage: chip %u reads %u writes; dma %u reads %u writes; %u transfers, %u bytes to the chip, %u bytes from the chip; completions %u taken (poll %u, timer %u, vblank %u), %u dropped without a routine",
                    chip_reads, chip_writes, dma_reads, dma_writes, transfers, bytes_to_chip, bytes_from_chip, taken[1] + taken[2] + taken[3], taken[1], taken[2], taken[3], dropped);
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
#ifdef PORT_HAVE_PSYZ
    Psyz_SpuInit();
#endif
    if (trace) {
        port_trace_line("iopage: the I/O page 0x%08x..0x%08x closed", IO_BASE, IO_END - 1);
        atexit(report);
    }
    return 0;
}
