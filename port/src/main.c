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

#include <stdlib.h>
#include <string.h>

#ifdef PORT_GUEST_VM
#include "../mac/platform.h"
#endif

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
    port_debug_end();
    port_suspenders_stop();
    port_suspenders_check_closed();
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
    const char *disc_path = NULL, *trace_path = NULL, *dump_path = NULL, *audio_dump = NULL;
    unsigned watchdog = 0, dump_every = 0;
    int audio_device = PORT_AUDIO_DEFAULT;
    const char *input_path = NULL;
    const char *settings_path = "settings.ini";
    int explicit_settings=0,show_settings=0,ui_override=-1,canvas_override=-1,center_override=-1,disc_override=-1;
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
        else if (strcmp(argv[i], "--pixel-perfect-ui") == 0) ui_override=1;
        else if (strcmp(argv[i], "--ps1-rendering") == 0) ui_override=0;
        else if (strcmp(argv[i], "--center-ui") == 0) center_override=1;
        else if (strcmp(argv[i], "--ps1-ui-layout") == 0) center_override=0;
        else if (strcmp(argv[i], "--fast-disc") == 0) disc_override=1;
        else if (strcmp(argv[i], "--ps1-disc-speed") == 0) disc_override=0;
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
        if(port_settings_load(settings_path,&port_settings,!explicit_settings,err,sizeof err))return refuse(err);
        if(ui_override>=0)port_settings.pixel_perfect_ui=ui_override;
        if(canvas_override>=0)port_settings.arcade_video=canvas_override;
        if(center_override>=0)port_settings.center_ui=center_override;
        if(disc_override>=0)port_settings.fast_disc=disc_override;
        if(show_settings) {
            if(disc_path || list)return refuse("--show-settings takes no disc or --list-library");
            printf("[experimental]\npixel_perfect_ui = %s\narcade_video = %s\ncenter_ui = %s\nfast_disc = %s\n",
                port_settings.pixel_perfect_ui?"true":"false",port_settings.arcade_video?"true":"false",port_settings.center_ui?"true":"false",
                port_settings.fast_disc?"true":"false");
            return 0;
        }
    }
    if (list && !bad && !disc_path && !trace && !trace_path && !input_path) {
        if (port_library_list(err, sizeof err) != 0) return refuse(err);
        return 0;
    }
    if (bad || list || !disc_path) {
        printf("usage: %s [--settings FILE] [--trace --trace-file FILE] [--input FILE] [--audio-dump FILE] [--audio | --no-audio] DISC (a .cue or the .bin itself)\n       %s --list-library | --show-settings\n", argc > 0 ? argv[0] : "sfa2", argc > 0 ? argv[0] : "sfa2");
        fflush(stdout);
        return 2;
    }
    if(port_gpu_set_video_options(port_settings.pixel_perfect_ui,port_settings.arcade_video) ||
       port_gpu_set_ui_layout(port_settings.center_ui))
        return refuse("experimental graphics require the patched PsyZ SDL_GPU backend");
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
#endif
    if (port_map(err, sizeof err) != 0) return refuse(err);
#ifdef PORT_GUEST_VM
    ram = vm_g(PORT_RAM_BASE, PORT_RAM_SIZE);
#endif
    if (port_mirror_init(trace, err, sizeof err) != 0) return refuse(err);
    if (port_iopage_init(audio_dump, audio_device, trace, err, sizeof err) != 0) return refuse(err);
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
    port_cd_set_fast(port_settings.fast_disc);
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
