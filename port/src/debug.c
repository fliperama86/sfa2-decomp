/* Debug aids that the run options switch on; nothing here is part of the game's behaviour.
 *
 * --dump-vram PREFIX  at the end of any run write the video memory to PREFIX_end.ppm
 * --dump-every N      also every N seconds of the run: PREFIX_00.ppm, PREFIX_01.ppm, ...
 *
 * The video memory is read back through port_gpu_read_frame (gpu.c) and written as a binary PPM
 * (1024x512, 15-bit colour widened to 8 bits). */
#include "gpu.h"

#include <stdlib.h>
#include <string.h>

static const char *prefix;
static unsigned every_sec;

void port_debug_set(const char *dump_prefix, unsigned dump_every)
{
    prefix = dump_prefix;
    every_sec = dump_every;
}

/* Writes only PREFIX_TAG.ppm, the path the user's prefix makes; a path that does not fit its buffer is not
 * written (a truncated name would be a different path). */
static void dump(const char *tag)
{
    unsigned x, y;
    unsigned short *px;
    unsigned char row[PORT_GPU_FRAME_W * 3];
    char path[1100];
    int n, failed = 0;
    FILE *f;

    n = snprintf(path, sizeof path, "%s_%s.ppm", prefix, tag);
    if (n < 0 || (size_t)n >= sizeof path) {
        printf("dump: the picture path (PREFIX_%s.ppm) is longer than %u characters; nothing written\n", tag, (unsigned)sizeof path - 1);
        fflush(stdout);
        return;
    }
    px = malloc((size_t)PORT_GPU_FRAME_W * PORT_GPU_FRAME_H * sizeof *px);
    if (!px) {
        printf("dump: no memory for the picture; %s not written\n", path);
        fflush(stdout);
        return;
    }
    if (port_gpu_read_frame(px) != 0) {
        printf("dump: the game has drawn nothing, or the window is gone; %s not written\n", path);
        fflush(stdout);
        free(px);
        return;
    }
    if (!(f = fopen(path, "wb"))) {
        printf("dump: cannot write %s\n", path);
        fflush(stdout);
        free(px);
        return;
    }
    if (fprintf(f, "P6\n%d %d\n255\n", PORT_GPU_FRAME_W, PORT_GPU_FRAME_H) < 0) failed = 1;
    for (y = 0; y < PORT_GPU_FRAME_H && !failed; y++) {
        for (x = 0; x < PORT_GPU_FRAME_W; x++) {
            unsigned v = px[y * PORT_GPU_FRAME_W + x];
            row[3 * x] = (unsigned char)(((v & 31) * 255) / 31);
            row[3 * x + 1] = (unsigned char)((((v >> 5) & 31) * 255) / 31);
            row[3 * x + 2] = (unsigned char)((((v >> 10) & 31) * 255) / 31);
        }
        if (fwrite(row, 1, sizeof row, f) != sizeof row) failed = 1;
    }
    if (fclose(f) != 0) failed = 1;
    if (failed) {
        printf("dump: writing %s failed; the file is incomplete\n", path);
        fflush(stdout);
    }
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
 * A thread that only watches. When no vblank has come for `seconds` it suspends the game's thread, reads its
 * instruction pointer and the words on its stack that point into the game's implementations, prints
 * one line, and ends the program with status 11. It never calls game code. */
#ifdef _WIN32
#include <windows.h>

static HANDLE game_thread;
static unsigned wd_seconds;

static DWORD WINAPI watch(LPVOID unused)
{
    unsigned last = port_frames();
    unsigned quiet = 0;
    (void)unused;
    for (;;) {
        Sleep(1000);
        if (port_frames() != last) {
            last = port_frames();
            quiet = 0;
            continue;
        }
        if (++quiet < wd_seconds) continue;
        {
            CONTEXT c;
            const unsigned *sp;
            int n = 0, k;
            char names[400] = "";
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
            ExitProcess(PORT_EXIT_HANG);
        }
    }
    return 0;
}

void port_debug_watchdog(unsigned seconds)
{
    if (!seconds) return;
    wd_seconds = seconds;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &game_thread, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CreateThread(NULL, 0, watch, NULL, 0, NULL);
}
#else
void port_debug_watchdog(unsigned seconds)
{
    (void)seconds;
}
#endif
