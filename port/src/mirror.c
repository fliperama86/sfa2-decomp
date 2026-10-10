/* The PS1's copy of RAM at address 0.
 *
 * The console shows its 2 MB of RAM a second time from address 0 and the first 64 KB of it hold the BIOS's
 * tables. Windows never lets a process have memory below 64 KB, so every access there faults, and this layer
 * serves it: the handler decodes the one faulting instruction (mirrorcore.c), carries it out on the mapped RAM
 * at 0x80000000 + address, moves the instruction pointer past it and resumes. Nothing of the game is replaced.
 *
 * Served: a read or a write, at an address that lies wholly in [0, 0x10000), by the game's thread, from an
 * instruction inside the game's own compiled code (between the build's two markers), of a form mirrorcore.c
 * serves. An access that begins below 0x10000 and ends at or above it, an access from anywhere else, an execute
 * fault and everything at 0x10000 and above is not touched: the crash line of main.c. A form that is not served
 * ends the program with `stop: crash: the game used the PS1's RAM mirror at 0x... (read|write) in NAME with an
 * instruction the port does not serve yet: BYTES`. A second fault while serving ends the program too.
 *
 * Above 0x10000 Windows has memory of its own in places that change from run to run, and an access there does not
 * fault, so it cannot be served: a read returns the system's bytes where the console read its RAM. This is a
 * known limit, not a handled case. The part below 0x10000 holds, on the console, the BIOS's tables; here it holds
 * zeros until the game or the port writes there.
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
        return EXCEPTION_CONTINUE_SEARCH;   /* it straddles 0x10000: the crash line */
    }

    port_mirror_exec(&in, &regs, (void *)(size_t)(PORT_RAM_BASE + addr));

    if (c->Eip != (DWORD)ip && !raced) {
        printf("stop: crash: the context of a fault in the PS1's RAM mirror is at 0x%08x, not at the faulting instruction 0x%08x\n", (unsigned)c->Eip, (unsigned)ip);
        fflush(stdout);
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

int port_mirror_init(int trace, char *err, size_t errsize)
{
    if (port_mirror_scan(query_page, err, errsize) != 0) return -1;
    tracing = (unsigned)trace;
    if (trace) {
        port_trace_line("mirror: start check: no page of 0x00000000..0x0000ffff is accessible");
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
