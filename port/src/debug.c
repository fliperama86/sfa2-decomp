/* Debug aid that a run option switches on; nothing here is part of the game's behaviour.
 *
 * --watchdog S  end a run that no longer reaches the frame clock, with a line that says where (below). */
#include "port.h"

#include <string.h>

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
#else
void port_debug_watchdog(unsigned seconds)
{
    (void)seconds;
}
#endif
