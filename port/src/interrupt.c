/* The vblank as an interrupt of the game's thread.
 *
 * On the console the vblank interrupts the game's code between any two instructions. Here a timer thread
 * does the same, bounded as the specification says:
 *  - the timer never calls game code, PsyZ, SDL or the C library: it measures time, suspends and resumes the
 *    game's thread, reads and sets its context, and reads flags;
 *  - the vblank is due by the one clock of kernel.c and taken exactly once, by the timer or by a library
 *    routine that ticks, whichever comes first;
 *  - the game's thread is interrupted only while its instruction pointer is in the game's own code (between
 *    the build's two marker symbols) or in the mapped PS1 RAM (a jump stands there), and never while a handler
 *    of the game runs or between EnterCriticalSection and ExitCriticalSection;
 *  - the interruption: the timer stores the interrupted instruction pointer in a variable and points the
 *    thread at port_interrupt_entry (assembly), which pushes it as a return address, saves the flags, every
 *    register and the FPU/SSE state, calls the vblank work in C on the game's own thread (so the handlers run
 *    there), puts everything back and returns to the interrupted instruction. The stack below the stack
 *    pointer is free on 32-bit x86 (no red zone), and the thread itself grows its stack for the routine.
 * Risk, stated: the game's code can now be interrupted between any two instructions, as on the console. */
#include "port.h"

#ifdef _WIN32
#include <windows.h>

extern char port_game_text_begin, port_game_text_end;

static HANDLE game_thread;
unsigned port_interrupt_count;               /* how many vblanks the timer delivered (for the controls and the page) */

void port_interrupt_entry(void);
#define PORT_STR2(x) #x
#define PORT_STR(x) PORT_STR2(x)
#define PORT_US PORT_STR(__USER_LABEL_PREFIX__)
__asm__(".text\n"
        ".globl " PORT_US "port_interrupt_entry\n" PORT_US "port_interrupt_entry:\n"
        "\tpushl " PORT_US "interrupted_eip_cell\n"   /* the return address: where the game was */
        "\tpushfl\n"
        "\tpushal\n"
        "\tmovl %esp, %ebp\n"
        "\tsubl $528, %esp\n"
        "\tandl $-16, %esp\n"
        "\tfxsave (%esp)\n"
        "\tcld\n"
        "\tcall " PORT_US "port_interrupt_work\n"
        "\tfxrstor (%esp)\n"
        "\tmovl %ebp, %esp\n"
        "\tpopal\n"
        "\tpopfl\n"
        "\tret\n");
/* the cell the assembly reads: the same variable under an assembler-visible name */
extern volatile unsigned interrupted_eip_cell;
volatile unsigned interrupted_eip_cell;

static int in_game_code(unsigned eip)
{
    return ((size_t)eip >= (size_t)&port_game_text_begin && (size_t)eip < (size_t)&port_game_text_end) ||
           (eip >= PORT_RAM_BASE && eip < PORT_RAM_BASE + PORT_RAM_SIZE);
}

static DWORD WINAPI timer(LPVOID unused)
{
    /* a high-resolution timer (CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, 0x2) where the system has it: Sleep(1) may last 15 ms */
    HANDLE wait = CreateWaitableTimerExW(NULL, NULL, 0x2, TIMER_ALL_ACCESS);
    (void)unused;
    for (;;) {
        LARGE_INTEGER due;
        due.QuadPart = -10000;   /* 1 ms */
        if (wait && SetWaitableTimer(wait, &due, 0, NULL, NULL, 0)) WaitForSingleObject(wait, 20);
        else Sleep(1);
        {
            CONTEXT c;
            if (SuspendThread(game_thread) == (DWORD)-1) continue;
            c.ContextFlags = CONTEXT_CONTROL;
            if (GetThreadContext(game_thread, &c) && in_game_code(c.Eip) && port_interrupt_allowed() && port_interrupt_take()) {
                interrupted_eip_cell = c.Eip;
                c.Eip = (DWORD)(size_t)port_interrupt_entry;
                SetThreadContext(game_thread, &c);
                port_interrupt_count++;
            }
            ResumeThread(game_thread);
        }
    }
    return 0;
}

void port_interrupt_start(void)
{
    if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &game_thread, 0, FALSE, DUPLICATE_SAME_ACCESS)) return;
    CreateThread(NULL, 0, timer, NULL, 0, NULL);
}
#else
void port_interrupt_start(void)
{
}
#endif
