/* Debug aids that the run options switch on; nothing here is part of the game's behaviour.
 *
 * --dump-vram PREFIX  at the end of any run write the video memory to PREFIX_end.ppm
 * --dump-every N      also every N seconds of the run: PREFIX_00.ppm, PREFIX_01.ppm, ...
 *
 * The video memory is read back through the graphics layer's StoreImage and written as a binary PPM
 * (1024x512, 15-bit colour widened to 8 bits). */
#include "port.h"

#include <stdlib.h>
#include <string.h>

static const char *prefix;
static unsigned every_sec;
static unsigned long long start_us;

void port_debug_set(const char *dump_prefix, unsigned dump_every)
{
    prefix = dump_prefix;
    every_sec = dump_every;
}

static void dump(const char *tag)
{
    typedef int (*store_t)(short *rect, void *pixels);
    store_t store = NULL;
    unsigned j;
    short rect[4] = { 0, 0, 1024, 512 };
    unsigned short *px;
    char path[1100];
    FILE *f;

    for (j = 0; port_gpu_library[j].name; j++)
        if (strcmp(port_gpu_library[j].name, "StoreImage") == 0) store = (store_t)port_gpu_library[j].host;
    px = malloc(1024 * 512 * 2);
    snprintf(path, sizeof path, "%s_%s.ppm", prefix, tag);
    if (!store || !px || !(f = fopen(path, "wb"))) {
        free(px);
        return;
    }
    store(rect, px);
    fprintf(f, "P6\n1024 512\n255\n");
    for (j = 0; j < 1024 * 512; j++) {
        unsigned v = px[j];
        fputc((int)(((v & 31) * 255) / 31), f);
        fputc((int)((((v >> 5) & 31) * 255) / 31), f);
        fputc((int)((((v >> 10) & 31) * 255) / 31), f);
    }
    fclose(f);
    free(px);
}

void port_debug_end(void)
{
    static int done;
    if (!prefix || done) return;
    done = 1;
    dump("end");
}

/* Called once per vblank by the frame clock, after the picture is presented; `seconds` since the start. */
void port_debug_tick(unsigned frame)
{
    static unsigned next_dump;
    (void)start_us;
    if (prefix && every_sec && frame >= next_dump * every_sec * 60) {
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
