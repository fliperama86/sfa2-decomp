/* sfa2.exe [--trace --trace-file FILE] DISC
 * sfa2.exe --list-library
 *
 * Map the PS1's memory, load the game's program from the disc image, write
 * the jumps (the game's C, the stop calls, then the library's host routines
 * and the overrides), and call the game's main.
 *
 * --settings FILE reads the settings file (settings.ini in the working directory by default; a missing default file gives the defaults, a
 *   missing or invalid explicit one refuses the start). port_settings_file is what the file gave and port_settings what the program uses:
 *   the file, then the command line. --show-settings prints both sections in the file's form for the effective settings, then `volume = N`,
 *   and ends. The command line overrides the file's keys, the last one winning: --sharp-bilinear and --nearest-scaling set [display]
 *   filter; --settings-menu and --no-settings-menu set [experimental] settings_menu (port/settings-menu.md); the others are in
 *   port/graphics.md. With the menu on, the menu's saves go to the file that was read (port_settings_path).
 * --list-library needs no disc: it prints the host routines, the ones that do
 * nothing on purpose, the library functions that still stop, and the overrides.
 * --trace with --trace-file FILE writes one line to FILE for every library call
 * (name and the first four argument words); off by default.
 *
 * Status (one `stop:` or `refused:` line says why, except for 0 by a return):
 *   0 main returned, or the window was closed
 *   2 a refusal at start (including the mirror's start check (mirror.c) and a bad table: a name listed twice, a listed
 *     name that no library function or function with C has)
 *   3 a function without C was reached
 *   4 a library function without a host routine was reached
 *   5 an unknown function was reached
 *   6 a disc command or state the disc layer does not handle, or a buffer the game named for it outside the
 *     PS1's RAM
 *   7 a graphics call or state the graphics layer does not handle
 *   8 a thread's function returned (the game never lets one)
 *   9 a library call whose arguments a host routine does not serve
 *  11 the watchdog (--watchdog S) found no vblank for S seconds; the line says where
 *  12 an address the game handed to the runtime to call (a thread's entry, an event handler, an interrupt or
 *     vsync callback) is not a function this program installed: `refused: PATH 0xADDRESS ...`, before the call
 *  13 Exec of a program of the disc, for which no C exists (--skip-programs continues instead), or a built-in program whose file, header or body is not the one its C was written from
 *  10 the program faulted (an access violation or the like); the line gives the address
 * --no-interrupt turns the timer thread off: the vblank is then taken only by the library routines that tick.
 * --timer-burst (a test mode; does nothing with --no-interrupt) makes the timer thread attempt its suspension of the game's thread
 *   without waiting between attempts and stay 1 ms in each round, and makes the stop routine of interrupt.c check its own contract:
 *   if a round was still in flight when it returned it prints `stop: a suspension round was in flight when the stop returned`
 *   and ends with status 10; if a round began after the stop it prints `stop: a suspension round began after the stop` (status 10); and each direct ExitProcess checks that the stop was called before it, with the line
 *   `stop: the process was ended while suspensions were still allowed` (status 10). Without the option those checks are not made.
 * Sound. A build with PsyZ opens the machine's sound output through PsyZ at start, without being asked (a device that cannot be
 *   opened is a line, not a stop). --no-audio opens none. --audio asks for it by name: the same in a build with PsyZ, and a refusal in
 *   a build without (which otherwise runs in silence and says nothing). --audio with --no-audio is the usage line.
 * --audio-dump FILE writes what the sound chip's model mixes to FILE: WAV, 16 bits, stereo, 44100 Hz, 735 frames at the end of every
 *   vblank (iopage.c). No audio device is opened then, with or without --audio (with it a line says so). It needs a build with PsyZ.
 * --input FILE gives the pad buttons that a run presses, on top of what PsyZ reads (input.c); a line of the file that is not
 *   valid refuses the start (status 2) with a line that names the file and the line number.
 * --skip-programs lets Exec of a program of the disc that has no C return at once, with a line at each skip (off: the run ends, status 13).
 *   A program whose C is built in (programs.c) runs either way.
 * --watchdog S ends the run with a line saying where the program is if no vblank came for S seconds (debug.c).
 * --dump-vram PREFIX writes the video memory to PREFIX_end.ppm at the end of any run; --dump-every N
 * also every N seconds (debug.c).
 * See PORT_EXIT_* in port.h. */
#include "port.h"
#include "gpu.h"
#include "port_tables.h"
#include "cd.h"
#include "modules.h"
#include "settings.h"
#include "intro.h"
#include "arcade.h"

#include <stdlib.h>
#include <string.h>

#ifdef PORT_GUEST_VM
#include "../mac/platform.h"
#endif

#if defined(_WIN32) || defined(PORT_LINUX)
#ifdef _WIN32
#include <windows.h>
#endif
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
#endif

#ifdef _WIN32
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
    port_debug_end();
    port_suspenders_stop();
    port_suspenders_check_closed();
    ExitProcess(PORT_EXIT_CRASH);
    return EXCEPTION_EXECUTE_HANDLER;
}
#elif defined(PORT_LINUX)
/* Linux (port/linux/README.md): one handler of SIGSEGV and SIGBUS gives each fault to the layers that serve faults (mirror.c,
 * modules.c), in the order they were added, and prints the Windows program's crash line for a fault that none serves. It runs on an
 * alternate stack of the game's thread, so that an overflow of a task's stack still gets its line, and with SA_NODEFER, so that a
 * fault inside a layer's work comes back here (mirror.c ends the program with its second-fault line then). The vblank's and the
 * watchdog's signals are blocked while it runs. */
#include <signal.h>
#include <stdint.h>
#include <ucontext.h>
#include <unistd.h>

#define FAULT_LAYERS 4
#define ALT_STACK 0x10000u
#define TRAP_PAGE_FAULT 14

static port_fault_layer fault_layers[FAULT_LAYERS];
static unsigned fault_layer_count;

int port_fault_add(port_fault_layer layer)
{
    if (fault_layer_count == FAULT_LAYERS) return -1;
    fault_layers[fault_layer_count++] = layer;
    return 0;
}

void port_exit_now(int status)
{
    _exit(status);
}

static void crash_end(void)
{
    fflush(stdout);
    port_debug_end();
    port_suspenders_stop();
    port_suspenders_check_closed();
    port_exit_now(PORT_EXIT_CRASH);
}

static void crashed(int sig, siginfo_t *si, void *context)
{
    ucontext_t *uc = context;
    size_t ip = (size_t)uc->uc_mcontext.gregs[REG_EIP];
    if ((sig == SIGSEGV || sig == SIGBUS) && uc->uc_mcontext.gregs[REG_TRAPNO] == TRAP_PAGE_FAULT) {
        /* the page fault's error code: bit 4 an instruction fetch, bit 1 a write */
        unsigned long code = (unsigned long)uc->uc_mcontext.gregs[REG_ERR];
        unsigned kind = code & 0x10 ? 8 : code & 0x2 ? 1 : 0, where = (unsigned)(uintptr_t)si->si_addr, i;
        const char *what = kind == 8 ? "execute" : kind ? "write" : "read";
        for (i = 0; i < fault_layer_count; i++)
            if (fault_layers[i](context, kind, where, (unsigned)ip)) return;
        if (where < PORT_RAM_SIZE)
            printf("stop: crash: the game used the PS1's RAM mirror at 0x%08x (%s) in %s (at 0x%08x)\n", where, what, port_function_at(ip), (unsigned)ip);
        else
            printf("stop: crash: access violation (%s 0x%08x) in %s (at 0x%08x)\n", what, where, port_function_at(ip), (unsigned)ip);
    } else
        printf("stop: crash: signal %d (trap %d) in %s (at 0x%08x)\n", sig, (int)uc->uc_mcontext.gregs[REG_TRAPNO], port_function_at(ip), (unsigned)ip);
    crash_end();
}

static int crash_install(char *err, size_t errsize)
{
    static const int sigs[] = { SIGSEGV, SIGBUS, SIGILL, SIGFPE, SIGTRAP };
    struct sigaction sa;
    stack_t alt;
    unsigned i;
    alt.ss_sp = malloc(ALT_STACK);
    alt.ss_size = ALT_STACK;
    alt.ss_flags = 0;
    if (!alt.ss_sp || sigaltstack(&alt, NULL) != 0) {
        snprintf(err, errsize, "faults: no alternate stack for the handler");
        return -1;
    }
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = crashed;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_NODEFER;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, PORT_SIG_VBLANK);
    sigaddset(&sa.sa_mask, PORT_SIG_WATCH);
    for (i = 0; i < sizeof sigs / sizeof sigs[0]; i++)
        if (sigaction(sigs[i], &sa, NULL) != 0) {
            snprintf(err, errsize, "faults: cannot install the handler of signal %d", sigs[i]);
            return -1;
        }
    return 0;
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
    const char *disc_path = NULL, *trace_path = NULL, *dump_path = NULL, *audio_dump = NULL;
    unsigned watchdog = 0, dump_every = 0;
    int audio_device = PORT_AUDIO_DEFAULT;
    const char *input_path = NULL;
    const char *settings_path = "settings.ini";
    int explicit_settings=0,show_settings=0,ui_override=-1,canvas_override=-1,center_override=-1,loads_override=-1,card_override=-1,filter_override=-1,menu_override=-1,intro_override=-1,assets_override=-1,arcade_check=0;
    const char *intro_path="intro.dat";
    const char *arcade_path="sfa2u.zip";   /* --arcade-rom FILE: the arcade set of the setting arcade_assets */
    const char *card1_path = "card1.mcr", *card2_path = NULL;   /* --card1, --card2: the card files of the setting memory_card */
    int volume = 100;   /* --volume PERCENT: the gain of the audio device's output */
    int list = 0, trace = 0, bad = 0, no_interrupt = 0, burst = 0, i;
    struct port_install installed;
    unsigned gp;
    FILE *trace_file = NULL;
    char err[PORT_ERR];
    struct port_disc disc;
    struct port_program prog;
#ifdef PORT_GUEST_VM
    unsigned char *ram = NULL;   /* the machine's RAM, once port_map has made it */
#else
    unsigned char *ram = (unsigned char *)(size_t)PORT_RAM_BASE;
#endif
    unsigned with_c, without_c, entry;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--list-library") == 0) list = 1;
        else if (strcmp(argv[i], "--settings") == 0 && i+1<argc && !explicit_settings) { settings_path=argv[++i];explicit_settings=1; }
        else if (strcmp(argv[i], "--show-settings") == 0) show_settings=1;
        else if (strcmp(argv[i], "--arcade-intro") == 0) intro_override=1;
        else if (strcmp(argv[i], "--ps1-intro") == 0) intro_override=0;
        else if (strcmp(argv[i], "--intro-data") == 0 && i+1<argc && argv[i+1][0]) intro_path=argv[++i];
        else if (strcmp(argv[i], "--arcade-assets") == 0) assets_override=1;
        else if (strcmp(argv[i], "--ps1-assets") == 0) assets_override=0;
        else if (strcmp(argv[i], "--arcade-rom") == 0 && i+1<argc && argv[i+1][0]) arcade_path=argv[++i];
        else if (strcmp(argv[i], "--arcade-check") == 0) arcade_check=1;
        else if (strcmp(argv[i], "--pixel-perfect-ui") == 0) ui_override=1;
        else if (strcmp(argv[i], "--ps1-rendering") == 0) ui_override=0;
        else if (strcmp(argv[i], "--center-ui") == 0) center_override=1;
        else if (strcmp(argv[i], "--ps1-ui-layout") == 0) center_override=0;
        else if (strcmp(argv[i], "--sharp-bilinear") == 0) filter_override=PORT_FILTER_SHARP_BILINEAR;
        else if (strcmp(argv[i], "--nearest-scaling") == 0) filter_override=PORT_FILTER_NEAREST;
        else if (strcmp(argv[i], "--settings-menu") == 0) menu_override=1;
        else if (strcmp(argv[i], "--no-settings-menu") == 0) menu_override=0;
        else if (strcmp(argv[i], "--volume") == 0 && i + 1 < argc) {
            char *end;
            long v = strtol(argv[++i], &end, 10);
            if (end == argv[i] || *end || v < 0 || v > 100) bad = 1;
            else volume = (int)v;
        }
        else if (strcmp(argv[i], "--memory-card") == 0) card_override=1;
        else if (strcmp(argv[i], "--no-memory-card") == 0) card_override=0;
        else if (strcmp(argv[i], "--card1") == 0 && i + 1 < argc && argv[i + 1][0]) card1_path = argv[++i];
        else if (strcmp(argv[i], "--card2") == 0 && i + 1 < argc && argv[i + 1][0]) card2_path = argv[++i];
        else if (strcmp(argv[i], "--fast-loads") == 0) loads_override=1;
        else if (strcmp(argv[i], "--ps1-loading") == 0) loads_override=0;
        else if (strcmp(argv[i], "--arcade-video") == 0) canvas_override=1;
        else if (strcmp(argv[i], "--ps1-video") == 0) canvas_override=0;
        else if (strcmp(argv[i], "--trace") == 0) trace = 1;
        else if (strcmp(argv[i], "--trace-file") == 0 && i + 1 < argc) trace_path = argv[++i];
        else if (strcmp(argv[i], "--audio-dump") == 0 && i + 1 < argc && !audio_dump) audio_dump = argv[++i];
        else if (strcmp(argv[i], "--audio") == 0 && audio_device != PORT_AUDIO_OFF) audio_device = PORT_AUDIO_ASKED;
        else if (strcmp(argv[i], "--no-audio") == 0 && audio_device != PORT_AUDIO_ASKED) audio_device = PORT_AUDIO_OFF;
        else if (strcmp(argv[i], "--no-interrupt") == 0) no_interrupt = 1;
        else if (strcmp(argv[i], "--timer-burst") == 0) burst = 1;
        else if (strcmp(argv[i], "--dump-vram") == 0 && i + 1 < argc) dump_path = argv[++i];
        else if (strcmp(argv[i], "--dump-every") == 0 && i + 1 < argc) dump_every = (unsigned)atoi(argv[++i]);
        else if (strcmp(argv[i], "--input") == 0 && i + 1 < argc && !input_path) input_path = argv[++i];
        else if (strcmp(argv[i], "--skip-programs") == 0) port_skip_programs = 1;
#ifdef PORT_GUEST_VM
        else if (strcmp(argv[i], "--guest") == 0 && i + 1 < argc) port_mac_image_path = argv[++i];
#endif
        else if (strcmp(argv[i], "--watchdog") == 0 && i + 1 < argc) watchdog = (unsigned)atoi(argv[++i]);
        else if (argv[i][0] != '-' && !disc_path) disc_path = argv[i];
        else bad = 1;
    }
    if (!bad) {
        /* port_settings_file is what the file gave, port_settings the program's: the file, then the command line. */
        if(port_settings_load(settings_path,&port_settings_file,!explicit_settings,err,sizeof err))return refuse(err);
        port_settings_path=settings_path;
        port_settings=port_settings_file;
        if(ui_override>=0)port_settings.pixel_perfect_ui=ui_override;
        if(canvas_override>=0)port_settings.arcade_video=canvas_override;
        if(center_override>=0)port_settings.center_ui=center_override;
        if(loads_override>=0)port_settings.fast_loads=loads_override;
        if(card_override>=0)port_settings.memory_card=card_override;
        if(filter_override>=0)port_settings.filter=filter_override;
        if(menu_override>=0)port_settings.settings_menu=menu_override;
        if(intro_override>=0)port_settings.arcade_intro=intro_override;
        if(assets_override>=0)port_settings.arcade_assets=assets_override;
        if(show_settings) {
            /* The file's own form: what port_settings_save writes, of the effective settings. */
            if(disc_path || list)return refuse("--show-settings takes no disc or --list-library");
            port_settings_write(stdout,&port_settings);
            printf("volume = %d\n", volume);
            return 0;
        }
    }
    if (list && !bad && !disc_path && !trace && !trace_path && !input_path) {
        if (port_library_list(err, sizeof err) != 0) return refuse(err);
        return 0;
    }
    if (!bad && !list && !disc_path && port_settings.arcade_assets) {
        /* Without a disc the arcade set must give everything; the loader says what it cannot give yet. */
        char line[PORT_ERR];
        if(port_arcade_configure(1,arcade_path,err,sizeof err))return refuse(err);
        port_arcade_missing(line, sizeof line);
        return refuse(line);
    }
    if (bad || list || !disc_path || (arcade_check && !port_settings.arcade_assets)) {
        printf("usage: %s [--settings FILE] [--sharp-bilinear | --nearest-scaling] [--settings-menu | --no-settings-menu] [--arcade-intro --intro-data FILE] [--arcade-assets --arcade-rom FILE [--arcade-check]] [--trace --trace-file FILE] [--input FILE] [--audio-dump FILE] [--audio | --no-audio] [--volume PERCENT] [--card1 FILE] [--card2 FILE] DISC (a .cue or the .bin itself)\n       %s --list-library | --show-settings\n", argc > 0 ? argv[0] : "sfa2", argc > 0 ? argv[0] : "sfa2");
        fflush(stdout);
        return 2;
    }
    if(port_intro_configure(port_settings.arcade_intro,intro_path,err,sizeof err))return refuse(err);
    if(port_arcade_configure(port_settings.arcade_assets,arcade_path,err,sizeof err))return refuse(err);
    if(port_gpu_set_video_options(port_settings.pixel_perfect_ui,port_settings.arcade_video) ||
       port_gpu_set_ui_layout(port_settings.center_ui) ||
       port_gpu_set_filter(port_settings.filter) ||
       port_gpu_set_screen(port_settings.screen,
           port_settings.screen==PORT_SCREEN_WINDOW?port_settings.window_w:port_settings.screen==PORT_SCREEN_FULLSCREEN?port_settings.fullscreen_w:0,
           port_settings.screen==PORT_SCREEN_WINDOW?port_settings.window_h:port_settings.screen==PORT_SCREEN_FULLSCREEN?port_settings.fullscreen_h:0))
        return refuse("experimental graphics require the patched PsyZ SDL_GPU backend");
    port_gpu_set_host_keys(port_settings.settings_menu);
    if (trace != (trace_path != NULL)) return refuse("--trace and --trace-file FILE go together");
    if (trace_path && !(trace_file = fopen(trace_path, "w"))) return refuse("cannot open the trace file");
    port_trace_set(trace_file);
    if (input_path) {
        char input_err[PORT_INPUT_ERR];
        if (port_input_load(input_path, input_err, sizeof input_err) != 0) return refuse(input_err);
    }
    port_debug_set(dump_path, dump_every);
    port_debug_watchdog(watchdog);
    atexit(port_debug_end);
#ifdef _WIN32
    SetUnhandledExceptionFilter(crashed);
#elif defined(PORT_LINUX)
    if (crash_install(err, sizeof err) != 0) return refuse(err);
#endif
    if (port_map(err, sizeof err) != 0) return refuse(err);
#ifdef PORT_GUEST_VM
    ram = vm_g(PORT_RAM_BASE, PORT_RAM_SIZE);
#endif
    if (port_mirror_init(trace, err, sizeof err) != 0) return refuse(err);
    if (port_iopage_init(audio_dump, audio_device, trace, err, sizeof err) != 0) return refuse(err);
    port_iopage_set_volume(volume);
    printf("memory: RAM at 0x%08x (2 MB), scratchpad at 0x%08x\n", PORT_RAM_BASE, PORT_SCRATCH);
    fflush(stdout);

    if (port_disc_open(&disc, disc_path, err, sizeof err) != 0) return refuse(err);
    printf("disc: %s, %u-byte sectors\n", disc.path, PORT_SECTOR);
    fflush(stdout);

    if (port_program_load(&disc, ram, port_program_sha256, &prog, err, sizeof err) != 0) return refuse(err);
    printf("program: %s at sector %u, %u bytes to 0x%08x, entry 0x%08x\n", prog.name, prog.sector, prog.t_size, prog.t_addr, prog.pc0);
    printf("identity: SHA-256 matches the build's baseline\n");
    fflush(stdout);
    if (arcade_check) {
        unsigned differ = port_arcade_program(ram, 1);
        int files_differ = port_arcade_attach(&disc, 1, err, sizeof err);
        if (files_differ < 0) return refuse(err);
        return differ || files_differ ? 1 : 0;
    }
    port_arcade_program(ram, 0);

    port_disc_handle = &disc;
    if (port_cd_init(&disc) != 0) return refuse("the disc layer cannot start");
    if (port_arcade_attach(&disc, 0, err, sizeof err) != 0) return refuse(err);
    port_cd_set_fast_waits(port_settings.fast_loads);
    port_gpu_set_batched_uploads(port_settings.fast_loads);
    if (port_card_configure(port_settings.memory_card, card1_path, card2_path, err, sizeof err) != 0) return refuse(err);
    atexit(port_card_flush);
    if (port_jumps_write(ram, &with_c, &without_c, err, sizeof err) != 0) return refuse(err);
    printf("jumps: %u written for functions with C, %u for functions without\n", with_c, without_c);
    fflush(stdout);

    if (port_library_install(ram, &installed, err, sizeof err) != 0) return refuse(err);
    printf("library: %u host routines, %u left that stop\n", installed.host, installed.stops);
    printf("overrides: %u\n", installed.overrides);
    {
        unsigned in_c = 0, k;
        for (k = 0; k < port_function_count; k++) in_c += port_functions[k].overridden != 0;
        printf("overrides in C: %u\n", in_c);
    }
    port_pad_print_keys();
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
    if (!no_interrupt) port_interrupt_start(burst);
#ifdef PORT_GUEST_VM
    port_call0(entry);
#else
    ((void (*)(void))(size_t)entry)();
#endif
    port_stop_main_returned();
    return 0;
}
