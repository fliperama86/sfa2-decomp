/* The port's runtime: shared declarations.
 *
 * Every function that can fail returns 0 for success, or -1 with one line
 * (no line end) in the caller's buffer saying why. Nothing here knows an
 * address of the game except the two ranges of the PS1's memory below.
 * disc.c uses plain stdio and no system feature, so a test can build it
 * with any C compiler; the system-specific part is memory.c alone. */
#ifndef PORT_H
#define PORT_H

#include <stddef.h>
#include <stdio.h>

#define PORT_RAM_BASE   0x80000000u
#define PORT_RAM_SIZE   0x00200000u
#define PORT_SCRATCH    0x1f800000u
#define PORT_SCRATCH_ALLOC 0x10000u   /* the system's granularity; the first 1 KB is the scratchpad */
#define PORT_SCRATCH_SIZE  0x400u     /* the scratchpad itself */
#define PORT_ERR        256           /* size of every error buffer */

/* memory.c: map the PS1's RAM (2 MB, executable) and scratchpad at their own
 * addresses. On failure the line names the range and the system's error. */
int  port_map(char *err, size_t errsize);
void port_unmap(void);

/* The memory that is the game's on this machine, for every layer that takes a pointer from the game (a structure to
 * read or write, a buffer to copy from or to). 1 when the whole span [p, p + n) lies in ONE of:
 *  - the mapped PS1 RAM;
 *  - the mapped scratchpad (its 1 KB);
 *  - the live part of the calling thread's own stack: from the frame of this very check up to the base of the stack the
 *    thread (or fiber) is running on now. The game's C is compiled natively, so its locals live on the host's stack
 *    and not in the mapped RAM; a pointer to one of them is the game's memory. Not the dead part below the check, not
 *    another task's stack (every task is a fiber with a stack of its own).
 * Nothing else: not the heap, not the program's own code or data. A span of 0 bytes is accepted when p itself lies in
 * one of these regions (not at the end of one). The sum is made in 64 bits. Not for the nodes of a drawing list: their
 * links are RAM addresses, so a node must be in the mapped RAM. */
int  port_game_span(const void *p, size_t n);

/* disc.c */
struct port_disc {
    FILE *f;
    char path[1024];           /* the image file actually read (the .bin) */
    unsigned long long length; /* its size in bytes */
};

#define PORT_SECTOR 2352u
#define PORT_DATA   2048u
#define PORT_DATA_OFFSET 24u

/* Open a .cue (first FILE "..." BINARY line, path relative to the cue's
 * folder) or a .bin itself. */
int  port_disc_open(struct port_disc *d, const char *path, char *err, size_t errsize);
void port_disc_close(struct port_disc *d);
/* Read the data bytes of the sector range starting at `sector`, `size` bytes. */
int  port_disc_read(struct port_disc *d, unsigned sector, unsigned size, unsigned char *out, char *err, size_t errsize);
/* Find a file by name in the root directory or one folder below it (version
 * suffix and case ignored). */
int  port_disc_find(struct port_disc *d, const char *name, unsigned *sector, unsigned *size, char *err, size_t errsize);
/* One file of the disc's directory: the name without its version suffix, lower
 * case; the first sector; the length in bytes. */
struct port_disc_file {
    char name[32];
    unsigned sector, size;
};
/* Every file of the root directory and of the folders one level below it, in
 * directory order, into out[0..max); *count says how many. A directory record
 * that runs past its sector or whose name runs past the record, a directory
 * larger than the bound, an extent beyond the image and more files than `max`
 * are each -1 with a line. */
int port_disc_list(struct port_disc *d, struct port_disc_file *out, unsigned max, unsigned *count, char *err, size_t errsize);
/* From the text of SYSTEM.CNF: the file name of its BOOT line. -1 if none. */
int  port_boot_name(const char *text, size_t length, char *out, size_t outsize);

struct port_program {
    char name[64];
    unsigned sector;    /* of the file on the disc */
    unsigned t_addr, t_size, pc0;
    unsigned b_addr, b_size;
};
/* Parse and load a PS-X EXE image of `size` bytes. `ram` points at the memory
 * that stands for PORT_RAM_BASE (the runtime passes the real address). */
int  port_exe_load(const unsigned char *file, size_t size, unsigned char *ram, struct port_program *p, char *err, size_t errsize);
/* SHA-256 of `size` bytes (sha256.c). */
void port_sha256(const unsigned char *data, size_t size, unsigned char out[32]);
/* SYSTEM.CNF, the boot file, its SHA-256 against `sha256` (the build's pinned
 * value), and only then port_exe_load of it: a program that differs is refused
 * before any byte of it is copied to `ram`. */
int  port_program_load(struct port_disc *d, unsigned char *ram, const unsigned char sha256[32], struct port_program *p, char *err, size_t errsize);
/* The target of the last jal before the first break (the entry code's halt
 * after main returns) within the first 64 instructions at pc0 of the loaded
 * memory. -1 if there is no break in the window or no jal before it. */
int  port_entry_scan(const unsigned char *ram, unsigned pc0, unsigned *target);

/* jumps.c */
/* Write the 5-byte jumps and calls for the resident functions. Counts go to
 * the two outputs. `ram` as above. */
/* Validate and sort the addresses of the resident entries (functions with C
 * and without), and keep the set. port_jumps_write calls this first. */
int  port_jump_set_build(char *err, size_t errsize);
/* Is `address` the start of a resident entry of the set (a place where a jump
 * is written)? The only addresses the runtime ever calls are such places. */
int  port_jump_known(unsigned address);
int  port_jumps_write(unsigned char *ram, unsigned *with_c, unsigned *without_c, char *err, size_t errsize);
/* Before the runtime calls an address that the game handed it (a thread's entry, an event handler, an
 * interrupt or vsync callback): accept it when it is the start of a resident entry the runtime wrote a jump or
 * a call at (a function with C, one without, a library function, an override), or a host address inside the
 * game's own compiled code (between the build's markers port_game_text_begin and port_game_text_end).
 * Anything else ends the program with `refused: PATH 0xADDRESS is not a function this program installed`
 * and status PORT_EXIT_TARGET; PATH names the call site. Called at the moment of the call. */
void port_target_check(const char *path, const void *target);
/* The entry of the 5-byte call written for functions without C. */
void port_stop_entry(void);
/* Print the line, flush, end the program. Never returns. */
void port_stop_main_returned(void);

/* ---- the library layer (library.c, kernel.c, threads.c, overrides.c) ---- */

/* Exit statuses. 0: main returned, or the window was closed. 2: a refusal at
 * start (one line says why). The others end a run with one `stop:` line that
 * names what is missing; a distinct status per kind. */
#define PORT_EXIT_REFUSED 2     /* refused: ... (before the game starts) */
#define PORT_EXIT_NO_C    3     /* a game function without C was reached */
#define PORT_EXIT_NO_HOST 4     /* a library function without a host routine was reached */
#define PORT_EXIT_UNKNOWN 5     /* a call into an unknown function */
#define PORT_EXIT_DISC    6     /* a disc command or state, or a buffer, that the disc layer does not handle */
#define PORT_EXIT_GRAPHICS 7    /* a graphics call or state the graphics layer does not handle */
#define PORT_EXIT_THREAD  8     /* a thread's function returned (the game never lets one) */
#define PORT_EXIT_CRASH   10    /* an unhandled fault (main.c prints where) */
#define PORT_EXIT_HANG    11    /* the watchdog found no vblank for its time limit */
#define PORT_EXIT_TARGET  12    /* refused: an address the game handed over is not an installed function */
#define PORT_EXIT_PROGRAM 13    /* Exec of a program of the disc for which no C exists (--skip-programs continues) */
#define PORT_EXIT_OTHER   9     /* a library call whose arguments a host routine does not serve */

/* Print `stop: ` and the formatted line, flush, end the program with `status`.
 * Never returns. For every host routine that meets something it cannot serve. */
void port_halt(int status, const char *fmt, ...);

/* One entry of a domain's table of host routines. `name` is the name the
 * build's tables give the library function (`ResetCallback`, or `func_8015760c`
 * where the project has no name), or `@0xADDRESS`: the library function at that
 * address, whatever the build calls it (the names are partly a matcher's guesses). `note` is NULL for a routine that does its
 * job; it is set, saying why that is right on a PC, for a routine that does
 * nothing on purpose. */
#define PORT_LIBRARY_DECLARED
struct port_library  { const char *name; void *host; const char *note; };
/* A host routine that replaces the C of a game function (by its name). */
struct port_override { const char *name; void *host; const char *note; };
struct port_domain   { const char *name; const struct port_library *table; };  /* table ends with a null name */

/* domains.c: the registry. The runtime's real one lists the six domains and the
 * overrides; a test supplies its own file with these definitions instead. */
extern const struct port_domain   port_domains[];   extern const unsigned port_domain_count;
extern const struct port_override *const port_override_sets[]; extern const unsigned port_override_set_count;  /* each set ends with a null name */

/* The domains' tables (ending with a null name). port_cd_library is defined by cd.c, port_gpu_library by gpu.c. */
extern const struct port_library port_kernel_library[], port_cd_library[], port_gpu_library[], port_sound_library[], port_card_library[],
                                 port_c_library[], port_thread_library[], port_system_library[];
extern const struct port_override port_game_overrides[];  /* overrides.c; ends with a null name */

struct port_install { unsigned host, noop, stops, overrides; };

/* Start writing trace lines (one per library call: name and the first four
 * argument words) to `f`; call before port_library_install. NULL: no trace. */
void port_trace_set(FILE *f);
/* After port_jumps_write: write the jumps of the host routines over the
 * stop calls of the library functions, and of the overrides over the game
 * functions' C. A refusal (a name listed twice, a listed name that no
 * absent library function or function with C has) is -1 with a line. */
int  port_library_install(unsigned char *ram, struct port_install *out, char *err, size_t errsize);
/* --list-library: print the groups (needs no memory and no disc). -1 with a line on a refusal. */
int  port_library_list(char *err, size_t errsize);

/* kernel.c */
/* Called by the host routines in which the game waits or polls (DrawSync,
 * VSync, GetRCnt, TestEvent, CdSync, CdReady, ...), and by the disc layer's own
 * waits. Keeps the clock of frames: when 1/60 s has passed it does one vblank
 * (the registered handlers, then port_cd_tick, then port_gpu_present); otherwise it yields the
 * processor briefly. A call made from inside a handler does nothing. */
void port_tick(void);
/* ResetCallback's work, for ResetGraph(0 or 3), which does it in PSY-Q. */
void port_callbacks_reset(void);
/* The BIOS's DeliverEvent: events open for (class, spec) and enabled get their
 * handler called, or are marked ready for TestEvent. */
void port_deliver_event(unsigned event_class, unsigned spec);
/* The port's own seam to the disc layer (cd.c). */
void port_cd_tick(void);

/* gpu.c: show the picture and pump the window's events; kernel.c calls it once per vblank. */
void port_gpu_present(void);

/* kernel.c / interrupt.c: the vblank as an interrupt of the game's thread */
extern volatile int port_handler_depth;   /* > 0 while a handler of the game runs; the disc layer raises it around its ready handler */
int  port_interrupt_allowed(void);
int  port_interrupt_take(void);
void port_clock_start(void);
/* Start the timer thread that interrupts the game's thread with the vblank (call from the game's thread, before the game starts). */
void port_interrupt_start(int burst);
/* interrupt.c: the gate that every suspension of the game's thread goes through. port_suspenders_stop ends them for good
 * (no suspension in flight afterwards, none can begin); it is registered with atexit and called before ExitProcess. The
 * enter/leave pair is for the watchdog of debug.c (enter returns 0 once stopped, and then holds nothing). */
void port_suspenders_init(void);   /* once, from the game's thread, before any thread that suspends it exists */
void port_suspenders_stop(void);
void port_suspenders_check_closed(void);   /* before each ExitProcess, after the stop: with --timer-burst, ends with a line if the gate is still open */
#ifdef _WIN32
int  port_suspenders_enter(void);
void port_suspenders_leave(void);
#endif

/* mirror.c: serve the PS1's copy of RAM below 0x200000 (see the file); main.c calls port_mirror_init before the game starts. */
int  port_mirror_init(int trace, char *err, size_t errsize);
/* interrupt.c: has the timer aimed the faulting thread at the interruption routine just as it faulted? The context's
 * instruction pointer is then the routine's, and the exception record holds either the routine's address or the
 * faulting instruction's; either way *fault_ip is the instruction that faulted. port_interrupt_set_return then moves the
 * address the interruption will return to. */
int  port_interrupt_aimed(unsigned context_eip, unsigned exception_ip, unsigned *fault_ip);
void port_interrupt_set_return(unsigned ip);

/* library.c: one line into the trace file, if tracing */
void port_trace_line(const char *fmt, ...);

/* debug.c: run options for looking at a run (see the file) */
void port_debug_set(const char *dump_prefix, unsigned dump_every);
void port_debug_end(void);
void port_debug_tick(unsigned frame);
void port_debug_watchdog(unsigned seconds);
const char *port_function_at(size_t ip);   /* main.c: the game function whose implementation is nearest at or below ip */
unsigned port_frames(void);                /* kernel.c: vblanks since the start */

/* main.c: the open disc, for the routines that look files up; --skip-programs */
extern struct port_disc *port_disc_handle;
extern int port_skip_programs;

/* threads.c */
/* The gp that the entry code loads (lui/addiu), from the loaded memory. */
int  port_entry_gp(const unsigned char *ram, unsigned pc0, unsigned *gp);
void port_set_gp(unsigned gp);

#endif
