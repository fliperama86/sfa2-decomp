/* sfa2.exe [--trace --trace-file FILE] DISC
 * sfa2.exe --list-library
 *
 * Map the PS1's memory, load the game's program from the disc image, write
 * the jumps (the game's C, the stop calls, then the library's host routines
 * and the overrides), and call the game's main.
 *
 * --list-library needs no disc: it prints the host routines, the ones that do
 * nothing on purpose, the library functions that still stop, and the overrides.
 * --trace with --trace-file FILE writes one line to FILE for every library call
 * (name and the first four argument words); off by default.
 *
 * Status (one `stop:` or `refused:` line says why, except for 0 by a return):
 *   0 main returned, or the window was closed
 *   2 a refusal at start (including a bad table: a name listed twice, a listed
 *     name that no library function or function with C has)
 *   3 a function without C was reached
 *   4 a library function without a host routine was reached
 *   5 an unknown function was reached
 *   6 a disc command or state the disc layer does not handle, or a buffer the game named for it outside the
 *     PS1's RAM
 *   8 a thread's function returned (the game never lets one)
 *   9 a library call whose arguments a host routine does not serve
 *  11 the watchdog (--watchdog S) found no vblank for S seconds; the line says where
 *  12 an address the game handed to the runtime to call (a thread's entry, an event handler, an interrupt or
 *     vsync callback) is not a function this program installed: `refused: PATH 0xADDRESS ...`, before the call
 *  13 Exec of a program of the disc, for which no C exists (--skip-programs continues instead)
 *  10 the program faulted (an access violation or the like); the line gives the address
 * --no-interrupt turns the timer thread off: the vblank is then taken only by the library routines that tick.
 * --skip-programs lets Exec of a program of the disc return at once, with a line at each skip (off: the run ends, status 13).
 * --watchdog S ends the run with a line saying where the program is if no vblank came for S seconds (debug.c).
 * See PORT_EXIT_* in port.h. */
#include "port.h"
#include "port_tables.h"
#include "cd.h"
#include "modules.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
/* A fault nobody handles ends the program with one line that says where, instead of the system's dialog. */
/* The game function whose implementation is the nearest at or below `ip` ("?" if none). */
const char *port_function_at(size_t ip)
{
    unsigned i, best = port_function_count;
    for (i = 0; i < port_function_count; i++)
        if (port_functions[i].impl && (size_t)port_functions[i].impl <= ip && (best == port_function_count || port_functions[i].impl > port_functions[best].impl)) best = i;
    /* an address far from any implementation is not in the game's code at all */
    if (best == port_function_count || ip - (size_t)port_functions[best].impl > 0x4000) return "?";
    return port_functions[best].name;
}

static LONG WINAPI crashed(EXCEPTION_POINTERS *p)
{
    const EXCEPTION_RECORD *r = p->ExceptionRecord;
    size_t ip = (size_t)r->ExceptionAddress;
    if (r->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && r->NumberParameters >= 2) {
        unsigned kind = (unsigned)r->ExceptionInformation[0], where = (unsigned)r->ExceptionInformation[1];
        const char *what = kind == 8 ? "execute" : kind ? "write" : "read";
        if (where < PORT_RAM_SIZE)
            printf("stop: crash: the game used the PS1's RAM mirror at 0x%08x (%s) in %s (at 0x%08x)\n", where, what, port_function_at(ip), (unsigned)ip);
        else
            printf("stop: crash: access violation (%s 0x%08x) in %s (at 0x%08x)\n", what, where, port_function_at(ip), (unsigned)ip);
    } else
        printf("stop: crash: exception 0x%08x in %s (at 0x%08x)\n", (unsigned)r->ExceptionCode, port_function_at(ip), (unsigned)ip);
    fflush(stdout);
    ExitProcess(PORT_EXIT_CRASH);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

struct port_disc *port_disc_handle;
int port_skip_programs;

static int refuse(const char *line)
{
    printf("refused: %s\n", line);
    fflush(stdout);
    return 2;
}

int main(int argc, char **argv)
{
    const char *disc_path = NULL, *trace_path = NULL;
    unsigned watchdog = 0;
    int list = 0, trace = 0, bad = 0, no_interrupt = 0, i;
    struct port_install installed;
    unsigned gp;
    FILE *trace_file = NULL;
    char err[PORT_ERR];
    struct port_disc disc;
    struct port_program prog;
    unsigned char *ram = (unsigned char *)(size_t)PORT_RAM_BASE;
    unsigned with_c, without_c, entry;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--list-library") == 0) list = 1;
        else if (strcmp(argv[i], "--trace") == 0) trace = 1;
        else if (strcmp(argv[i], "--trace-file") == 0 && i + 1 < argc) trace_path = argv[++i];
        else if (strcmp(argv[i], "--no-interrupt") == 0) no_interrupt = 1;
        else if (strcmp(argv[i], "--skip-programs") == 0) port_skip_programs = 1;
        else if (strcmp(argv[i], "--watchdog") == 0 && i + 1 < argc) watchdog = (unsigned)atoi(argv[++i]);
        else if (argv[i][0] != '-' && !disc_path) disc_path = argv[i];
        else bad = 1;
    }
    if (list && !bad && !disc_path && !trace && !trace_path) {
        if (port_library_list(err, sizeof err) != 0) return refuse(err);
        return 0;
    }
    if (bad || list || !disc_path) {
        printf("usage: %s [--trace --trace-file FILE] DISC (a .cue or the .bin itself)\n       %s --list-library\n", argc > 0 ? argv[0] : "sfa2", argc > 0 ? argv[0] : "sfa2");
        fflush(stdout);
        return 2;
    }
    if (trace != (trace_path != NULL)) return refuse("--trace and --trace-file FILE go together");
    if (trace_path && !(trace_file = fopen(trace_path, "w"))) return refuse("cannot open the trace file");
    port_trace_set(trace_file);
    port_debug_watchdog(watchdog);
#ifdef _WIN32
    SetUnhandledExceptionFilter(crashed);
#endif
    if (port_map(err, sizeof err) != 0) return refuse(err);
    printf("memory: RAM at 0x%08x (2 MB), scratchpad at 0x%08x\n", PORT_RAM_BASE, PORT_SCRATCH);
    fflush(stdout);

    if (port_disc_open(&disc, disc_path, err, sizeof err) != 0) return refuse(err);
    printf("disc: %s, %u-byte sectors\n", disc.path, PORT_SECTOR);
    fflush(stdout);

    if (port_program_load(&disc, ram, port_program_sha256, &prog, err, sizeof err) != 0) return refuse(err);
    printf("program: %s at sector %u, %u bytes to 0x%08x, entry 0x%08x\n", prog.name, prog.sector, prog.t_size, prog.t_addr, prog.pc0);
    printf("identity: SHA-256 matches the build's baseline\n");
    fflush(stdout);

    port_disc_handle = &disc;
    if (port_cd_init(&disc) != 0) return refuse("the disc layer cannot start");
    if (port_jumps_write(ram, &with_c, &without_c, err, sizeof err) != 0) return refuse(err);
    printf("jumps: %u written for functions with C, %u for functions without\n", with_c, without_c);
    fflush(stdout);

    if (port_library_install(ram, &installed, err, sizeof err) != 0) return refuse(err);
    printf("library: %u host routines, %u left that stop\n", installed.host, installed.stops);
    printf("overrides: %u\n", installed.overrides);
    fflush(stdout);

    port_modules_init(prog.t_addr, prog.t_size);
    if (port_entry_gp(ram, prog.pc0, &gp) != 0) return refuse("start: no lui/addiu of gp among the first 64 instructions at the entry");
    port_set_gp(gp);
    if (port_entry_scan(ram, prog.pc0, &entry) != 0) return refuse("start: no jal before a break among the first 64 instructions at the entry");
    if (!port_jump_known(entry)) {
        char line[PORT_ERR];
        snprintf(line, sizeof line, "the entry 0x%08x is not a function this build knows", entry);
        return refuse(line);
    }
    printf("start: 0x%08x\n", entry);
    fflush(stdout);

    port_clock_start();
    if (!no_interrupt) port_interrupt_start();
    ((void (*)(void))(size_t)entry)();
    port_stop_main_returned();
    return 0;
}
