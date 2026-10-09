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
#define PORT_ERR        256           /* size of every error buffer */

/* memory.c: map the PS1's RAM (2 MB, executable) and scratchpad at their own
 * addresses. On failure the line names the range and the system's error. */
int  port_map(char *err, size_t errsize);
void port_unmap(void);

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
#define PORT_EXIT_THREAD  8     /* a thread's function returned (the game never lets one) */
#define PORT_EXIT_CRASH   10    /* an unhandled fault (main.c prints where) */
#define PORT_EXIT_HANG    11    /* the watchdog found no vblank for its time limit */
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
struct port_library  { const char *name; void *host; const char *note; };
/* A host routine that replaces the C of a game function (by its name). */
struct port_override { const char *name; void *host; const char *note; };
struct port_domain   { const char *name; const struct port_library *table; };  /* table ends with a null name */

/* domains.c: the registry. The runtime's real one lists the six domains and the
 * overrides; a test supplies its own file with these definitions instead. */
extern const struct port_domain   port_domains[];   extern const unsigned port_domain_count;
extern const struct port_override *const port_override_sets[]; extern const unsigned port_override_set_count;  /* each set ends with a null name */

/* The domains' tables (ending with a null name). */
extern const struct port_library port_kernel_library[], port_sound_library[], port_card_library[],
                                 port_c_library[], port_thread_library[], port_system_library[];
extern const struct port_override port_game_overrides[];  /* overrides.c; ends with a null name */

struct port_install { unsigned host, noop, stops, overrides; };

/* Start writing trace lines (one per library call: name and the first four
 * argument words) to `f`; call before port_library_install. NULL: no trace. */
void port_trace_set(FILE *f);
/* After port_jumps_write: write the jumps of the host routines over the
 * stop calls of the library functions, and of the overrides over the game
 * functions' C or, for a function without C, over its stop call. A refusal
 * (a name listed twice, a library name that no absent library function has,
 * an override name that is in neither the functions nor the absents) is -1
 * with a line. */
int  port_library_install(unsigned char *ram, struct port_install *out, char *err, size_t errsize);
/* --list-library: print the groups (needs no memory and no disc). -1 with a line on a refusal. */
int  port_library_list(char *err, size_t errsize);

/* kernel.c */
/* Called by the host routines in which the game waits or polls (VSync,
 * GetRCnt, TestEvent, ...). Keeps the clock of frames: when 1/60 s has passed
 * it does one vblank (the registered handlers); otherwise it yields the
 * processor briefly. A call made from inside a handler does nothing. */
void port_tick(void);
/* ResetCallback's work, for ResetGraph(0 or 3), which does it in PSY-Q. */
void port_callbacks_reset(void);
/* The BIOS's DeliverEvent: events open for (class, spec) and enabled get their
 * handler called, or are marked ready for TestEvent. */
void port_deliver_event(unsigned event_class, unsigned spec);

/* kernel.c: the frame clock */
extern volatile int port_handler_depth;   /* > 0 while a handler of the game runs */
void port_clock_start(void);

/* library.c: one line into the trace file, if tracing */
void port_trace_line(const char *fmt, ...);

/* debug.c: the watchdog (see the file) */
void port_debug_watchdog(unsigned seconds);
const char *port_function_at(size_t ip);   /* main.c: the game function whose implementation is nearest at or below ip */
unsigned port_frames(void);                /* kernel.c: vblanks since the start */

/* threads.c */
/* The gp that the entry code loads (lui/addiu), from the loaded memory. */
int  port_entry_gp(const unsigned char *ram, unsigned pc0, unsigned *gp);
void port_set_gp(unsigned gp);

#endif
