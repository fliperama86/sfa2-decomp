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

#include "guest.h"

/* The native Linux program (port/linux/README.md): a 32-bit x86 process, like the Windows program. A 64-bit build on Linux (the
 * controls that compile a runtime file with the machine's own compiler) is not it and gets the files' other branches. */
#if defined(__linux__) && defined(__i386__) && !defined(PORT_GUEST_VM)
#define PORT_LINUX 1
#endif

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

/* The port's extra memory: PORT_EXTRA_SIZE bytes after the RAM, which the console does not have and the game's code knows
 * nothing of. The arcade loader puts there what is larger than the room the PS1 gives it (port/arcade/README.md, "Full
 * animation sets") and the game reaches it through the 32-bit offsets of its own data. port_map_extra maps it readable and
 * writable (0, or -1 with a line: taken, or not written for this system yet); port_seal_extra makes it read-only, so that a
 * write of the game there is the crash line rather than a silent change. Both are called by the loader only, and unmapped by
 * port_unmap. */
#define PORT_EXTRA_BASE 0x80200000u
#define PORT_EXTRA_SIZE 0x00100000u
int  port_map_extra(char *err, size_t errsize);
void port_seal_extra(void);

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
struct port_pack;
struct port_disc {
    FILE *f;
    char path[1024];           /* the image file actually read (the .bin), or the pack */
    unsigned long long length; /* its size in bytes (of the disc it stands for, with a pack) */
    struct port_pack *pack;    /* a pack of the arcade loader in place of the image (port/arcade/README.md), or null */
};

#define PORT_SECTOR 2352u
#define PORT_DATA   2048u
#define PORT_DATA_OFFSET 24u

/* Open a .cue (first FILE "..." BINARY line, path relative to the cue's
 * folder) or a .bin itself. */
int  port_disc_open(struct port_disc *d, const char *path, char *err, size_t errsize);
void port_disc_close(struct port_disc *d);
/* Open a pack of the arcade loader as the disc: its sectors are the image's but for the bytes the loader builds. */
int  port_disc_open_pack(struct port_disc *d, const char *path, char *err, size_t errsize);
/* The whole raw sector (2352 bytes) of the image, or of the pack (its sectors' headers made anew for the data files). -1 past the
 * end or for a sector the pack does not hold. */
int  port_disc_raw(struct port_disc *d, unsigned sector, unsigned char *raw);
/* With a pack: the ranges of the file at `sector` that the loader built equal to the disc's when the pack was made (pairs of offset
 * and size, sorted) and the SHA-256 of their bytes. 0, or -1 when the pack has no such file. */
int  port_disc_pack_built(struct port_disc *d, unsigned sector, const unsigned (**ranges)[2], unsigned *count, const unsigned char **digest);
/* With a pack: the music codes (file * 256 + channel of a ZERO2_XA file) whose streams it holds silent. 0, or -1 without a pack. */
int  port_disc_pack_silent(struct port_disc *d, const unsigned short **codes, unsigned *count);
/* Read the data bytes of the sector range starting at `sector`, `size` bytes. With port_disc_patch_hook set (the arcade
 * loader), each sector's bytes then pass through it, which may replace them with what the loader built. */
extern void (*port_disc_patch_hook)(unsigned sector, unsigned char *data, unsigned n);
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
/* Set by the modules layer: is `address` an entry of a module that the layer installed (it wrote that entry's own
 * jump, or call to the stop, and the five bytes are still exactly that)? The module is placed first if the address's
 * bytes came from a pinned chunk of the disc and its page is not placed yet. 1: yes. 0: no. -1: it was installed and
 * its jump is no longer there. Null until the layer starts. port_target_check asks it for an address that is not a
 * resident entry. */
extern int (*port_module_known)(unsigned address);
/* Set by the modules layer, read by the timer thread of interrupt.c: is the page at `address` non-executable
 * because the disc wrote it? */
extern int (*port_page_blocked)(unsigned address);
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
#define PORT_EXIT_PROGRAM 13    /* Exec of a program of the disc for which no C exists (--skip-programs continues), or a built-in program that fails its checks (programs.c) */
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
/* `enabled`, if not null, points to a setting: the override is installed only while it is not 0 (an experimental setting). */
struct port_override { const char *name; void *host; const char *note; const int *enabled; };
struct port_domain   { const char *name; const struct port_library *table; };  /* table ends with a null name */

/* domains.c: the registry. The runtime's real one lists the six domains and the
 * overrides; a test supplies its own file with these definitions instead. */
extern const struct port_domain   port_domains[];   extern const unsigned port_domain_count;
extern const struct port_override *const port_override_sets[]; extern const unsigned port_override_set_count;  /* each set ends with a null name */

/* The domains' tables (ending with a null name). port_cd_library is defined by cd.c, port_gpu_library by gpu.c. */
extern const struct port_library port_kernel_library[], port_cd_library[], port_gpu_library[], port_sound_library[], port_card_library[],
                                 port_c_library[], port_thread_library[], port_system_library[];
extern const struct port_override port_game_overrides[];  /* overrides.c; ends with a null name */
extern const struct port_override port_loadwait_overrides[];  /* loadwait.c; ends with a null name; installed only with the setting fast_loads */
extern const struct port_override port_menu_overrides[];  /* menu_run.c; ends with a null name; installed only with the setting settings_menu */
extern const struct port_override port_wide_object_overrides[];  /* wide_objects.c; ends with a null name; always placed, the game's rules unchanged */
extern const struct port_override port_wide_overrides[];  /* wide_stage.c; ends with a null name; always placed, run the game's C alone with widescreen off */
/* library.c: the C of a function of the game (resident or a module's), by name, to call from an override (port_call0 ... port_call4);
 * PORT_FN_NULL when the build has none. */
port_fn port_function_impl(const char *name);
extern const struct port_override port_arcade_overrides[];  /* arcade.c; ends with a null name; installed only with the setting arcade_assets */  /* menu_run.c; ends with a null name; installed only with the setting settings_menu */

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
/* The fast clock of the setting fast_loads (kernel.c). port_clock_hurry(1)/(0): while a fast load loop runs, port_tick does not sleep for the next vblank
 * (they nest). port_clock_blank_start(n): for at most n frames, or until port_clock_frame_drawn is given a frame of PORT_CLOCK_RICH_PACKETS packets or
 * more, a vblank that is not due is made due. The packets of a loading screen are about 30, a menu or a fight 200 and up. */
#define PORT_CLOCK_RICH_PACKETS 150u
void port_clock_hurry(int on);
void port_clock_blank_start(unsigned max_frames);
void port_clock_frame_drawn(unsigned packets);
/* The settings menu (menu_run.c, port/settings-menu.md): port_clock_hurrying, is a fast load loop running; port_clock_paused(us), the menu
 * held the game for `us`: the one clock leaves that time out, so nothing of the game's falls due for it and no vblank is owed. */
int port_clock_hurrying(void);
void port_clock_paused(unsigned long long us);
/* For the watchdog (debug.c): grows at every vblank and at every frame of the settings menu (port_clock_menu_frame). */
void port_clock_menu_frame(void);
unsigned port_alive(void);
/* gpu.c: PsyZ batches the image uploads of one command buffer (port/psyz.patch), for the setting fast_loads. No effect without PsyZ. */
void port_gpu_set_batched_uploads(int on);
/* ResetCallback's work, for ResetGraph(0 or 3), which does it in PSY-Q. */
void port_callbacks_reset(void);
/* Marks the callback layer as not started (see kernel.c); port_program_exec calls it before a built-in program's setup. */
void port_callbacks_program_start(void);
/* The BIOS's DeliverEvent: events open for (class, spec) and enabled get their
 * handler called, or are marked ready for TestEvent. */
void port_deliver_event(unsigned event_class, unsigned spec);
/* card.c: the memory cards of the setting memory_card. on 0: no cards (the game is told the slots are empty). on 1: slot 1 is the 128 KB card file at path1 (made
 * when the game first changes it, if it is not there), slot 2 the file at path2, or empty when path2 is NULL. -1 and a line in err for a file of another size. port_card_flush
 * writes the cards that have unwritten changes (the game's close and format already do). */
int port_card_configure(int on, const char *path1, const char *path2, char *err, size_t errsize);
void port_card_flush(void);
/* The port's own seam to the disc layer (cd.c). */
void port_cd_tick(void);
/* A ready handler of the runtime's own for the disc layer (cd.c): called for each delivered data sector with (1, result) and for a disc
 * error with (5, result) in place of the game's handler variable. Null removes it; port_CdInit removes it too. */
void port_cd_host_ready(void (*handler)(int intr, unsigned char *result));
/* For a stream's code in the runtime. port_cd_fifo_read: the next `bytes` bytes of the current sector's FIFO into dst (the caller's own
 * buffer, not checked), by the rules of port_CdGetSector (same position, zeros once dry); returns how many were the sector's bytes and
 * stores the sector's number in *sector (if not null; -1 with no sector in the FIFO). It does not touch the record of sources nor the
 * hook. port_cd_store: `bytes` bytes at src into the game's memory at madr as bytes of the disc's sector `sector` (-1: from nowhere):
 * madr checked as CdGetSector's is, the record of sources and the hook as for it; 0 bytes do nothing. */
unsigned port_cd_fifo_read(void *dst, unsigned bytes, int *sector);
void port_cd_store(void *madr, const void *src, unsigned bytes, int sector);

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
#if defined(_WIN32) || defined(PORT_LINUX)
int  port_suspenders_enter(void);
void port_suspenders_leave(void);
#endif

#ifdef PORT_LINUX
/* memory.c (Linux): the console's copy of RAM is mapped from here to 0x200000, the same pages as the RAM's own; below it the system
 * maps nothing (vm.mmap_min_addr) and mirror.c serves the game's accesses as faults. */
#define PORT_MIRROR_LOW 0x10000u
/* The real-time signals that interrupt the game's thread (they need <signal.h>): the vblank's timer (interrupt.c) and the watchdog
 * (debug.c). Both are blocked while main.c's fault handler runs. */
#define PORT_SIG_VBLANK (SIGRTMIN)
#define PORT_SIG_WATCH  (SIGRTMIN + 1)
/* main.c (Linux): the one handler of SIGSEGV and SIGBUS hands every fault to the layers added here, in the order they were added;
 * the first that returns 1 has served it and the program resumes with the context as that layer left it. `context` is the signal's
 * ucontext_t; kind 0 read, 1 write, 8 execute (a Windows access violation's numbers); addr the address accessed; ip the faulting
 * instruction's. A fault that no layer serves prints main.c's crash line and ends the program. -1 when the table is full. */
typedef int (*port_fault_layer)(void *context, unsigned kind, unsigned addr, unsigned ip);
int port_fault_add(port_fault_layer layer);
/* main.c (Linux): end the program at once with `status`, running no exit handler: the Windows program's ExitProcess. */
void port_exit_now(int status);
/* threads.c (Linux): the address just above the stack of the task that runs now (memory.c's port_game_span). */
unsigned long port_stack_top(void);
#endif

/* mirror.c: serve the PS1's copy of RAM below 0x200000 (see the file); main.c calls port_mirror_init before the game starts. */
int  port_mirror_init(int trace, char *err, size_t errsize);
/* interrupt.c: has the timer aimed the faulting thread at the interruption routine just as it faulted? The context's
 * instruction pointer is then the routine's, and the exception record holds either the routine's address or the
 * faulting instruction's; either way *fault_ip is the instruction that faulted. port_interrupt_set_return then moves the
 * address the interruption will return to. */
int  port_interrupt_aimed(unsigned context_eip, unsigned exception_ip, unsigned *fault_ip);
void port_interrupt_set_return(unsigned ip);

/* iopage.c: the console's I/O page (three cells, and with PsyZ the sound chip and DMA channel 4), served as faults by mirror.c's
 * handler. port_iopage_init: close the page, in every build, after port_map; with PsyZ start its model of the chip. read /
 * check_write / write return 0 or -1 with the reason in `why` (mirror.c ends the program with it); check_write is called before
 * the instruction that writes is carried out, write after it; served: once per fault served.
 * The completion of a transfer (see iopage.c), taken exactly once: port_iopage_poll by a poll of the game (kernel.c has checked
 * the vblank's rules), port_iopage_deliver by the vblank's delivery before its handlers (port_handler_depth already raised),
 * port_iopage_claim_due by the timer thread (1: claimed) and port_iopage_deliver_claimed by the interruption routine. */
/* What the command line said about the machine's sound output (main.c). DEFAULT: nothing; a build with PsyZ opens it, one without runs
 * in silence. ASKED: --audio; as DEFAULT with PsyZ, a refusal without. OFF: --no-audio; never opened. */
#define PORT_AUDIO_DEFAULT (-1)
#define PORT_AUDIO_OFF     0
#define PORT_AUDIO_ASKED   1
int  port_iopage_init(const char *audio_dump, int audio_device, int trace, char *err, size_t errsize);   /* audio_dump: a file name or NULL; audio_device: PORT_AUDIO_* */
/* The gain of the sound output, 0 to 100 percent (--volume). With PsyZ it is set on PsyZ's one audio stream (port/psyz.patch); the audio dump is not changed. No effect in a build without PsyZ. */
void port_iopage_set_volume(int percent);
int  port_iopage_read(unsigned addr, unsigned size, unsigned *value, char *why, size_t whysize);
int  port_iopage_check_write(unsigned addr, unsigned size, char *why, size_t whysize);
int  port_iopage_write(unsigned addr, unsigned size, unsigned value, char *why, size_t whysize);
void port_iopage_served(void);
void port_iopage_vblank(void);   /* kernel.c, once per vblank: with --trace, the counts line every 60 vblanks when the counts changed */
void port_iopage_poll(void);
void port_iopage_deliver(void);
int  port_iopage_claim_due(void);
void port_iopage_deliver_claimed(void);
/* kernel.c: the one clock, in microseconds. iopage.c: PSY-Q's DMACallback(channel, routine): keeps the routine, returns the previous. */
unsigned long long port_now_us(void);
void *port_h_DMACallback(int dma, port_fn func);
port_fn port_iopage_dma_callback(int dma);   /* the routine registered for a channel (0 to 6, else null) */
/* press.c: the movie decoder's output transfer, the completion in the pattern of the sound transfer's (see press.h). port_press_poll takes
 * a pending completion whose due time has passed (kernel.c, port_tick, with the sound transfer's poll); claim_due and deliver_claimed are
 * the timer thread's claim and the interruption routine's delivery. */
void port_press_poll(void);
int  port_press_claim_due(void);
void port_press_deliver_claimed(void);
void port_press_report(const char *program);   /* programs.c, when a built-in program has returned: one line of counts (press.h) */

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

/* input.c: the pads (see the file). The BIOS's frame of one controller is PORT_PAD_FRAME bytes. */
#define PORT_PAD_FRAME 34
void port_pad_host_init(void);                               /* once per InitPAD */
void port_pad_host_read(unsigned char *dst, int len);        /* port 1's frame, the first min(len, PORT_PAD_FRAME) bytes */
void port_pad_print_keys(void);                              /* one line at start when PsyZ is linked, else nothing */
#define PORT_INPUT_ERR        2048   /* size of the error buffer that port_input_load needs: a long path is shortened to PORT_INPUT_PATH_SHOWN characters */
#define PORT_INPUT_PATH_SHOWN 1000
int  port_input_load(const char *path, char *err, size_t errsize);   /* --input FILE: 0, or -1 with a line that names the file and line */
int  port_input_loaded(void);
void port_input_apply(unsigned char *frame, unsigned vblank);        /* press the script's buttons held at that vertical blank on a digital pad frame */
/* The host keys of the script (f1 escape f4 guide) pressed since the last call, as PORT_INPUT_KEY_* (the same bits as gpu.h's PORT_GPU_KEY_*). */
#define PORT_INPUT_KEY_MENU       1u
#define PORT_INPUT_KEY_ESCAPE     2u
#define PORT_INPUT_KEY_FULLSCREEN 4u
#define PORT_INPUT_KEY_GUIDE      8u
unsigned port_input_take_keys(void);

/* threads.c */
/* The gp that the entry code loads (lui/addiu), from the loaded memory. */
int  port_entry_gp(const unsigned char *ram, unsigned pc0, unsigned *gp);
void port_set_gp(unsigned gp);

#endif
