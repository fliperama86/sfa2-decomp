/* Debug aids that the run options switch on; nothing here is part of the game's behaviour.
 *
 * --dump-vram PREFIX  at the end of any run write the video memory to PREFIX_end.ppm
 * --dump-every N      also every N seconds of the run: PREFIX_00.ppm, PREFIX_01.ppm, ...
 *
 * The video memory is read back through port_gpu_read_frame (gpu.c) and written as a binary PPM
 * (1024x512, 15-bit colour widened to 8 bits). With the setting widescreen (port/widescreen.md) each picture also writes
 * PREFIX_wide_TAG.ppm, the wide canvas of the frame shown last (port_gpu_read_wide), in the same form and its own size; nothing
 * when that frame was not shown wide. */
#include "gpu.h"
#include "video.h"

#include <stdlib.h>
#include <string.h>

static const char *prefix;
static unsigned every_sec;

void port_debug_set(const char *dump_prefix, unsigned dump_every)
{
    prefix = dump_prefix;
    every_sec = dump_every;
}

/* The path PREFIX_TAG.ppm, or PREFIX_wide_TAG.ppm for the wide canvas (wide = 1): the path the user's prefix makes. A path that does
 * not fit its buffer is not written (a truncated name would be a different path); 0 then, else 1. */
static int make_path(char *path, size_t size, const char *kind, const char *tag)
{
    int n = snprintf(path, size, "%s_%s%s.ppm", prefix, kind, tag);
    if (n < 0 || (size_t)n >= size) {
        printf("dump: the picture path (PREFIX_%s%s.ppm) is longer than %u characters; nothing written\n", kind, tag, (unsigned)size - 1);
        fflush(stdout);
        return 0;
    }
    return 1;
}

/* Writes the w x h picture px (15-bit colour halfwords) as a binary PPM, widened to 8 bits. */
static void write_ppm(const char *path, const unsigned short *px, unsigned w, unsigned h)
{
    unsigned x, y;
    unsigned char *row = malloc((size_t)w * 3);
    int failed = 0;
    FILE *f;

    if (!row) {
        printf("dump: no memory for the picture; %s not written\n", path);
        fflush(stdout);
        return;
    }
    if (!(f = fopen(path, "wb"))) {
        printf("dump: cannot write %s\n", path);
        fflush(stdout);
        free(row);
        return;
    }
    if (fprintf(f, "P6\n%u %u\n255\n", w, h) < 0) failed = 1;
    for (y = 0; y < h && !failed; y++) {
        for (x = 0; x < w; x++) {
            unsigned v = px[y * w + x];
            row[3 * x] = (unsigned char)(((v & 31) * 255) / 31);
            row[3 * x + 1] = (unsigned char)((((v >> 5) & 31) * 255) / 31);
            row[3 * x + 2] = (unsigned char)((((v >> 10) & 31) * 255) / 31);
        }
        if (fwrite(row, 1, (size_t)w * 3, f) != (size_t)w * 3) failed = 1;
    }
    if (fclose(f) != 0) failed = 1;
    if (failed) {
        printf("dump: writing %s failed; the file is incomplete\n", path);
        fflush(stdout);
    }
    free(row);
}

/* Writes PREFIX_TAG.ppm, the video memory, and PREFIX_wide_TAG.ppm, the wide canvas of the frame shown last when there is one
 * (setting widescreen, port/widescreen.md): its own size, nothing when the frame was not shown wide. */
static void dump(const char *tag)
{
    unsigned short *px;
    char path[1100];
    int w = 0, h = 0;

    if (!make_path(path, sizeof path, "", tag)) return;
    px = malloc((size_t)PORT_GPU_FRAME_W * PORT_GPU_FRAME_H * sizeof *px);
    if (!px) {
        printf("dump: no memory for the picture; %s not written\n", path);
        fflush(stdout);
        return;
    }
    if (port_gpu_read_frame(px) != 0) {
        printf("dump: the game has drawn nothing, or the window is gone; %s not written\n", path);
        fflush(stdout);
    } else {
        write_ppm(path, px, PORT_GPU_FRAME_W, PORT_GPU_FRAME_H);
    }
    free(px);
    px = malloc((size_t)PORT_WIDE_W * PORT_ARCADE_H * sizeof *px);
    if (px && port_gpu_read_wide(px, &w, &h) == 0 && w > 0 && h > 0 && w <= PORT_WIDE_W && h <= PORT_ARCADE_H &&
        make_path(path, sizeof path, "wide_", tag))
        write_ppm(path, px, (unsigned)w, (unsigned)h);
    free(px);
}

void port_debug_end(void)
{
    static int done;
    if (!prefix || done) return;
    done = 1;
    dump("end");
}

/* Called once per vblank by the frame clock, after the picture is presented; `frame` counts vblanks. The next
 * dump is due at frame (next_dump * every_sec * 60), computed in 64 bits so that a large N does not wrap. */
void port_debug_tick(unsigned frame)
{
    static unsigned next_dump;
    if (prefix && every_sec && frame >= (unsigned long long)next_dump * every_sec * 60ull) {
        char tag[16];
        snprintf(tag, sizeof tag, "%02u", next_dump++);
        dump(tag);
    }
}

/* ---- the watchdog: reports where a run that no longer reaches the frame clock is, and ends it ----
 * A thread that only watches. When no vblank (and no frame of the settings menu, port_alive) has come for `seconds` it suspends the game's thread, reads its
 * instruction pointer and the words on its stack that point into the game's implementations, prints
 * one line, and ends the program with status 11. It never calls game code. */
#ifdef _WIN32
#include <windows.h>

static HANDLE game_thread;
static unsigned wd_seconds;

static DWORD WINAPI watch(LPVOID unused)
{
    unsigned last = port_alive();
    unsigned quiet = 0;
    (void)unused;
    for (;;) {
        Sleep(1000);
        if (port_alive() != last) {
            last = port_alive();
            quiet = 0;
            continue;
        }
        if (++quiet < wd_seconds) continue;
        {
            CONTEXT c;
            const unsigned *sp;
            int n = 0, k;
            char names[400] = "";
            if (!port_suspenders_enter()) return 0;   /* the process is ending already */
            SuspendThread(game_thread);
            c.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
            GetThreadContext(game_thread, &c);
            sp = (const unsigned *)(size_t)c.Esp;
            for (k = 0; k < 256 && n < 6; k++) {
                const char *name = port_function_at(sp[k]);
                if (name[0] != '?' && strlen(names) + strlen(name) + 4 < sizeof names) {
                    strcat(names, n ? ", " : "");
                    strcat(names, name);
                    n++;
                }
            }
            printf("stop: hang: no vblank for %u s; the program is at 0x%08x in %s; stack words point into: %s\n", quiet, (unsigned)c.Eip, port_function_at(c.Eip), names);
            fflush(stdout);
            ExitProcess(PORT_EXIT_HANG);   /* the gate stays held: nothing may resume the game's thread */
        }
    }
    return 0;
}

void port_debug_watchdog(unsigned seconds)
{
    if (!seconds) return;
    wd_seconds = seconds;
    port_suspenders_init();
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &game_thread, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CreateThread(NULL, 0, watch, NULL, 0, NULL);
}
#elif defined(PORT_LINUX)
/* Linux (port/linux/README.md): no thread can suspend another, so the watchdog sends the game's thread a signal, whose handler stores
 * the thread's instruction and stack pointers and keeps the thread parked in the handler (so its stack stays as it was) until the
 * watchdog has printed its line and ended the program. A thread that does not answer within a second gets a line without the place. */
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>

static pthread_t game_thread;
static unsigned wd_seconds;
static volatile int wd_answered;
static volatile unsigned wd_eip, wd_esp;

static void watch_signal(int sig, siginfo_t *si, void *context)
{
    ucontext_t *uc = context;
    (void)sig;
    (void)si;
    wd_eip = (unsigned)uc->uc_mcontext.gregs[REG_EIP];
    wd_esp = (unsigned)uc->uc_mcontext.gregs[REG_ESP];
    wd_answered = 1;
    for (;;) pause();   /* parked: the watchdog ends the process */
}

static void *watch(void *unused)
{
    unsigned last = port_alive();
    unsigned quiet = 0;
    (void)unused;
    for (;;) {
        sleep(1);
        if (port_alive() != last) {
            last = port_alive();
            quiet = 0;
            continue;
        }
        if (++quiet < wd_seconds) continue;
        {
            struct timespec wait = { 0, 10000000 };
            const unsigned *sp;
            int n = 0, k;
            char names[400] = "";
            if (!port_suspenders_enter()) return NULL;   /* the process is ending already */
            pthread_kill(game_thread, PORT_SIG_WATCH);
            for (k = 0; k < 100 && !wd_answered; k++) nanosleep(&wait, NULL);
            if (!wd_answered) {
                printf("stop: hang: no vblank for %u s; the program's place is not known (its thread did not answer)\n", quiet);
                fflush(stdout);
                port_exit_now(PORT_EXIT_HANG);
            }
            sp = (const unsigned *)(size_t)wd_esp;
            for (k = 0; k < 256 && n < 6; k++) {
                const char *name = port_function_at(sp[k]);
                if (name[0] != '?' && strlen(names) + strlen(name) + 4 < sizeof names) {
                    strcat(names, n ? ", " : "");
                    strcat(names, name);
                    n++;
                }
            }
            printf("stop: hang: no vblank for %u s; the program is at 0x%08x in %s; stack words point into: %s\n", quiet, wd_eip, port_function_at(wd_eip), names);
            fflush(stdout);
            port_exit_now(PORT_EXIT_HANG);   /* the gate stays held */
        }
    }
    return NULL;
}

void port_debug_watchdog(unsigned seconds)
{
    struct sigaction sa;
    pthread_t t;
    if (!seconds) return;
    wd_seconds = seconds;
    port_suspenders_init();
    game_thread = pthread_self();
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = watch_signal;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, PORT_SIG_VBLANK);
    if (sigaction(PORT_SIG_WATCH, &sa, NULL) != 0) return;
    if (pthread_create(&t, NULL, watch, NULL) == 0) pthread_detach(t);
}
#else
void port_debug_watchdog(unsigned seconds)
{
    (void)seconds;
}
#endif
