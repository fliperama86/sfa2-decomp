/* The PS1's copy of RAM at address 0.
 *
 * The console shows its 2 MB of RAM a second time from address 0 and the first 64 KB of it hold the BIOS's
 * tables. This layer serves every access of the game to [0, 0x200000): the handler decodes the one faulting
 * instruction (mirrorcore.c), carries it out on the mapped RAM at 0x80000000 + address, moves the instruction
 * pointer past it and resumes. Nothing of the game is replaced.
 *
 * Why every access faults. Windows never lets a process have memory below 64 KB. From 0x10000 to 0x1fffff it
 * would put memory of its own, so the program takes the range first: it is linked with its image base at
 * 0x10000, not relocatable, and a filler section (`.hole`, uninitialized) from 0x11000 up to its first real
 * section at 0x200000 (hostbuild.py has the link settings and checks the header). The range then belongs to the
 * program's image and nobody else can allocate there. At start (port_mirror_init) the filler is made
 * PAGE_NOACCESS, and the start check asks the system about every page of [0, 0x200000): one accessible page, and
 * the program refuses to start and names it. So a program linked the old way refuses. The image's header page
 * (0x10000..0x10fff) is the one page that stays readable: the C library reads the executable's header at exit and
 * a closed page ends the program inside the library. An access of the game to that page is not served: a read
 * returns the header's bytes and is not seen, a write is the crash line.
 *
 * Served: a read or a write, at an address that lies wholly in [0, 0x10000) or [0x11000, 0x200000), by the game's thread, from an
 * instruction inside the game's own compiled code (between the build's two markers), of a form mirrorcore.c
 * serves. An access that begins below 0x200000 and ends at or above it, an access from anywhere else, an
 * execute fault and everything at 0x200000 and above is not touched: the crash line of main.c (0x200000 is the
 * program's first section: a read there returns the program's own bytes and does not fault). A form that is
 * not served ends the program with `stop: crash: the game used the PS1's RAM mirror at 0x... (read|write) in
 * NAME with an instruction the port does not serve yet: BYTES`. A second fault while serving ends the program too.
 *
 * What the copy holds: zeros at first; on the console the first 64 KB hold the BIOS's tables, so an accidental
 * read like [null + 0xd] reads a zero where the console read a byte of the BIOS's.
 *
 * Seen, not silent: the first use of the copy by each function prints one line; with --trace each access and
 * the start check are lines of the trace file, and at exit the count per function.
 *
 * The vblank (interrupt.c) does not redirect the thread while this handler works: the timer redirects only a thread
 * whose instruction pointer is in the game's code or in the PS1's RAM, and the handler is neither. The window
 * between the fault and the handler's start, in which the timer can still aim the thread, is dealt with in serve(). */
#include "port.h"
#include "mirror.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

static volatile int port_mirror_serving;   /* set while serve() works: a fault of the game's thread then is a second fault */

extern char port_game_text_begin, port_game_text_end;

#define MAX_SEEN 128
static struct { const char *name; unsigned reads, writes; } seen[MAX_SEEN];
static unsigned seen_count, game_tid, tracing;
unsigned port_mirror_raced;   /* vblanks the timer aimed at the thread between the fault and this handler (for the controls) */
unsigned port_mirror_served_count;

static int query_page(unsigned addr, int *accessible, unsigned *next)
{
    MEMORY_BASIC_INFORMATION m;
    if (!VirtualQuery((void *)(size_t)addr, &m, sizeof m)) return -1;
    *accessible = m.State == MEM_COMMIT && !(m.Protect & (PAGE_NOACCESS | PAGE_GUARD));
    *next = (unsigned)(size_t)m.BaseAddress + (unsigned)m.RegionSize;
    if (*next <= addr) *next = addr + 0x1000;
    return 0;
}

static void report(void)
{
    unsigned i;
    for (i = 0; i < seen_count; i++)
        port_trace_line("mirror: %s: %u reads, %u writes", seen[i].name, seen[i].reads, seen[i].writes);
}

static void stop_unserved(const char *what, unsigned where, const char *name, const char *why, const unsigned char *code, unsigned avail)
{
    char bytes[64];
    port_mirror_hex(code, avail > 15 ? 15 : avail, bytes, sizeof bytes);
    printf("stop: crash: the game used the PS1's RAM mirror at 0x%08x (%s) in %s %s: %s\n", where, what, name, why, bytes);
    fflush(stdout);
    port_suspenders_stop();
    port_suspenders_check_closed();
    ExitProcess(PORT_EXIT_CRASH);
}

static int in_game_code(size_t ip)
{
    return ip >= (size_t)&port_game_text_begin && ip < (size_t)&port_game_text_end;
}

static LONG CALLBACK serve(EXCEPTION_POINTERS *p)
{
    const EXCEPTION_RECORD *rec = p->ExceptionRecord;
    CONTEXT *c = p->ContextRecord;
    size_t ip;
    unsigned kind, addr, avail, i;
    struct mirror_regs regs;
    struct mirror_insn in;
    const unsigned char *code;
    const char *name, *what;
    int raced;
    unsigned aimed_ip = 0;

    if (rec->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || rec->NumberParameters < 2) return EXCEPTION_CONTINUE_SEARCH;
    if (port_mirror_serving && GetCurrentThreadId() == game_tid) {
        /* a second fault while serving */
        printf("stop: crash: a second fault (%s 0x%08x) while the PS1's RAM mirror was being served\n",
               rec->ExceptionInformation[0] == 8 ? "execute" : rec->ExceptionInformation[0] ? "write" : "read", (unsigned)rec->ExceptionInformation[1]);
        fflush(stdout);
        port_suspenders_stop();
        port_suspenders_check_closed();
        ExitProcess(PORT_EXIT_CRASH);
    }
    kind = (unsigned)rec->ExceptionInformation[0];
    addr = (unsigned)rec->ExceptionInformation[1];
    ip = (size_t)rec->ExceptionAddress;
    /* The vblank's timer may have aimed the thread at port_interrupt_entry in the instant between the fault and
     * this handler (it saw the thread at the faulting instruction, in game code). The context then holds the
     * entry as its instruction pointer, and the exception record holds the entry's address or the instruction's.
     * The instruction is carried out all the same, and the entry's cell is moved past it, so the interruption
     * returns after the instruction. */
    raced = port_interrupt_aimed((unsigned)c->Eip, (unsigned)ip, &aimed_ip);
    if (raced) ip = aimed_ip;
    if (!port_mirror_decision(kind, addr, 1, GetCurrentThreadId() == game_tid, in_game_code(ip))) return EXCEPTION_CONTINUE_SEARCH;

    port_mirror_serving = 1;
    name = port_function_at(ip);
    what = kind ? "write" : "read";
    code = (const unsigned char *)ip;
    avail = (unsigned)((size_t)&port_game_text_end - ip);
    regs.r[0] = c->Eax; regs.r[1] = c->Ecx; regs.r[2] = c->Edx; regs.r[3] = c->Ebx;
    regs.r[4] = c->Esp; regs.r[5] = c->Ebp; regs.r[6] = c->Esi; regs.r[7] = c->Edi;
    regs.eip = (unsigned)ip;
    regs.eflags = c->EFlags;
    if (port_mirror_decode(code, avail, &regs, &in) != 0)
        stop_unserved(what, addr, name, "with an instruction the port does not serve yet", code, avail);
    if (in.ea != addr) {
        char why[80];
        snprintf(why, sizeof why, "with an instruction whose operand is at 0x%08x, not there", in.ea);
        stop_unserved(what, addr, name, why, code, avail);
    }
    if (!port_mirror_decision(kind, addr, in.msize, 1, 1)) {
        port_mirror_serving = 0;
        return EXCEPTION_CONTINUE_SEARCH;   /* it straddles the end of the copy: the crash line */
    }

    port_mirror_exec(&in, &regs, (void *)(size_t)(PORT_RAM_BASE + addr));

    if (c->Eip != (DWORD)ip && !raced) {
        printf("stop: crash: the context of a fault in the PS1's RAM mirror is at 0x%08x, not at the faulting instruction 0x%08x\n", (unsigned)c->Eip, (unsigned)ip);
        fflush(stdout);
        port_suspenders_stop();
        port_suspenders_check_closed();
        ExitProcess(PORT_EXIT_CRASH);
    }
    c->Eax = regs.r[0]; c->Ecx = regs.r[1]; c->Edx = regs.r[2]; c->Ebx = regs.r[3];
    c->Esp = regs.r[4]; c->Ebp = regs.r[5]; c->Esi = regs.r[6]; c->Edi = regs.r[7];
    c->EFlags = regs.eflags;
    if (raced) {
        port_interrupt_set_return(regs.eip);
        port_mirror_raced++;
    } else {
        c->Eip = regs.eip;
    }
    port_mirror_served_count++;

    for (i = 0; i < seen_count && seen[i].name != name; i++) {
    }
    if (i == seen_count && seen_count < MAX_SEEN) {
        seen[seen_count].name = name;
        seen[seen_count].reads = seen[seen_count].writes = 0;
        seen_count++;
        printf("mirror: %s uses the PS1's copy of RAM at address 0 (first at 0x%08x; %s)\n", name, addr, what);
        fflush(stdout);
    }
    if (i < seen_count) {
        if (kind) seen[i].writes++;
        else seen[i].reads++;
    }
    if (tracing) port_trace_line("mirror: %s %s 0x%08x %u (at 0x%08x)", name, what, addr, in.msize, (unsigned)ip);
    port_mirror_serving = 0;
    return EXCEPTION_CONTINUE_EXECUTION;
}

/* Close the filler section of the program's own image, when the image has it (the header says so). The header
 * page stays readable: the C library reads the executable's header when the program exits, and a closed page
 * ends the program with an access violation inside the library. An image without the filler is left alone: the
 * start check then finds the system's pages and refuses. */
static int close_own_range(unsigned *closed_end, char *err, size_t errsize)
{
    const unsigned char *base = (const unsigned char *)GetModuleHandle(NULL);
    unsigned end = 0;
    DWORD old;
    *closed_end = 0;
    if ((size_t)base != MIRROR_IMAGE_BASE) return 0;
    if (port_mirror_image_hole(base, 0x1000, &end) != 0) return 0;
    if (!VirtualProtect((void *)(size_t)MIRROR_HOLE, end - MIRROR_HOLE, PAGE_NOACCESS, &old)) {
        snprintf(err, errsize, "mirror: cannot close the program's own range 0x%08x..0x%08x (system error %lu)", MIRROR_HOLE, end - 1, (unsigned long)GetLastError());
        return -1;
    }
    *closed_end = end;
    return 0;
}

int port_mirror_init(int trace, char *err, size_t errsize)
{
    unsigned closed_end;
    if (close_own_range(&closed_end, err, errsize) != 0) return -1;
    if (port_mirror_scan(query_page, closed_end ? MIRROR_IMAGE_BASE : 0, closed_end ? MIRROR_HOLE : 0, err, errsize) != 0) return -1;
    tracing = (unsigned)trace;
    if (trace) {
        if (closed_end) port_trace_line("mirror: the program's own range 0x%08x..0x%08x closed; its header page 0x%08x..0x%08x stays readable", MIRROR_HOLE, closed_end - 1, MIRROR_IMAGE_BASE, MIRROR_HOLE - 1);
        port_trace_line("mirror: start check: no page of 0x00000000..0x%08x is accessible%s", MIRROR_LIMIT - 1, closed_end ? " (the header page excepted)" : "");
        atexit(report);
    }
    game_tid = GetCurrentThreadId();
    if (!AddVectoredExceptionHandler(1, serve)) {
        snprintf(err, errsize, "mirror: no access fault handler for this system");
        return -1;
    }
    return 0;
}

#else

int port_mirror_init(int trace, char *err, size_t errsize)
{
    (void)trace;
    (void)err;
    (void)errsize;
    return 0;
}

#endif
