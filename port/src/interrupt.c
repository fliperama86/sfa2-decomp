/* The vblank as an interrupt of the game's thread.
 *
 * On the console the vblank interrupts the game's code between any two instructions. Here a timer thread
 * does the same, within these bounds:
 *  - the timer never calls game code, PsyZ, SDL or the C library's streams: it measures time, suspends and
 *    resumes the game's thread, reads and sets its context, and reads flags;
 *  - the vblank is due by the one clock of kernel.c and taken exactly once, by the timer or by a library
 *    routine that ticks, whichever comes first;
 *  - the game's thread is interrupted only while its instruction pointer is in the game's own code (between
 *    the build's two marker symbols) or in the mapped PS1 RAM (a jump stands there), and never while a handler
 *    of the game runs or between EnterCriticalSection and ExitCriticalSection;
 *  - the interruption: the timer stores the interrupted instruction pointer in a variable and points the
 *    thread at port_interrupt_entry (assembly), which pushes it as a return address, saves the flags, every
 *    register and the FPU/SSE state, gives C a clean floating-point state and a clear direction flag, calls the vblank work in C on the game's own thread (so the handlers run
 *    there), puts everything back and returns to the interrupted instruction. The stack below the stack
 *    pointer is free on 32-bit x86 (no red zone), and the thread itself grows its stack for the routine.
 * Risk, stated: the game's code can now be interrupted between any two instructions, as on the console. */
#include "port.h"

#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

extern char port_game_text_begin, port_game_text_end;

static HANDLE game_thread;
static int burst;
unsigned port_interrupt_count;               /* how many vblanks the timer delivered (for the controls and the page) */

void port_interrupt_entry(void);
#define PORT_STR2(x) #x
#define PORT_STR(x) PORT_STR2(x)
#define PORT_US PORT_STR(__USER_LABEL_PREFIX__)
/* What the routine gives the C handler: the environment that the C calling convention promises a function at
 * its entry, all of it. After the interrupted state is saved (flags, general registers, and by fxsave the x87
 * registers, tag, control and status words, and MXCSR) it sets: an empty x87 stack with the default control word
 * (fninit: 0x037f, extended precision, round to nearest, exceptions masked); the default MXCSR (0x1f80, the
 * interrupted code may have changed rounding or unmasked exceptions); the direction flag clear (cld: the
 * interrupted code may be inside a string operation with DF set); and the stack 16-byte aligned at the call
 * (esp is rounded down with `andl $-16`, so the call leaves the callee with esp = 12 mod 16, which is what the
 * i386 ABI of the compiler expects at a function's entry). On return fxrstor puts back the interrupted x87
 * state and MXCSR and popfl the flags, so the interrupted code gets exactly its own back.
 * Deliberately not saved: the segment registers (C here never changes them), the upper halves of the AVX
 * registers (fxsave does not hold them and neither the game's code nor the handler's compiled C uses AVX;
 * libraries that dispatch to AVX at run time are called from the handler, a risk that is noted, not handled),
 * and debug registers. */
__asm__(".text\n"
        ".globl " PORT_US "port_interrupt_entry\n" PORT_US "port_interrupt_entry:\n"
        "\tpushl " PORT_US "interrupted_eip_cell\n"   /* the return address: where the game was */
        "\tpushfl\n"
        "\tpushal\n"
        "\tmovl %esp, %ebp\n"
        "\tsubl $528, %esp\n"
        "\tandl $-16, %esp\n"
        "\tfxsave (%esp)\n"
        "\tfninit\n"
        "\tpushl $0x1f80\n"
        "\tldmxcsr (%esp)\n"
        "\taddl $4, %esp\n"
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

int port_interrupt_aimed(unsigned context_eip, unsigned exception_ip, unsigned *fault_ip)
{
    if (context_eip != (unsigned)(size_t)port_interrupt_entry) return 0;
    if (exception_ip == context_eip) {            /* the system took the thread's address after the redirection */
        *fault_ip = interrupted_eip_cell;
        return 1;
    }
    if (interrupted_eip_cell == exception_ip) {   /* the system had taken it before */
        *fault_ip = exception_ip;
        return 1;
    }
    return 0;
}

void port_interrupt_set_return(unsigned ip)
{
    interrupted_eip_cell = ip;
}

static int in_game_code(unsigned eip)
{
    return ((size_t)eip >= (size_t)&port_game_text_begin && (size_t)eip < (size_t)&port_game_text_end) ||
           (eip >= PORT_RAM_BASE && eip < PORT_RAM_BASE + PORT_RAM_SIZE);
}

/* The gate. Whoever suspends the game's thread (the timer here, the watchdog of debug.c) holds the gate from before
 * SuspendThread until after ResumeThread. port_suspenders_stop takes the gate once and sets the flag under it; after
 * it returns no suspension is in flight and none can begin. It runs at every way the game's thread ends the process:
 * registered with atexit (every exit() and a return from main) and called before the ExitProcess of main.c's crash
 * routine. Without it the timer could be ended by ExitProcess between its SuspendThread and its ResumeThread, and the
 * game's thread, left suspended inside the exit, would never finish ending the process. */
static CRITICAL_SECTION gate;
static int gate_ready, stopped;

void port_suspenders_init(void)
{
    if (gate_ready) return;
    InitializeCriticalSection(&gate);
    gate_ready = 1;
    atexit(port_suspenders_stop);
}

void port_suspenders_stop(void)
{
    if (!gate_ready) return;
    EnterCriticalSection(&gate);
    stopped = 1;
    LeaveCriticalSection(&gate);
}

int port_suspenders_enter(void)
{
    EnterCriticalSection(&gate);
    if (stopped) {
        LeaveCriticalSection(&gate);
        return 0;
    }
    return 1;
}

void port_suspenders_leave(void)
{
    LeaveCriticalSection(&gate);
}

static DWORD WINAPI timer(LPVOID unused)
{
    /* a high-resolution timer (CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, 0x2) where the system has it: Sleep(1) may last 15 ms */
    HANDLE wait = CreateWaitableTimerExW(NULL, NULL, 0x2, TIMER_ALL_ACCESS);
    (void)unused;
    for (;;) {
        LARGE_INTEGER due;
        due.QuadPart = -10000;   /* 1 ms */
        if (burst) { /* --timer-burst: no wait between attempts (a control) */ }
        else if (wait && SetWaitableTimer(wait, &due, 0, NULL, NULL, 0)) WaitForSingleObject(wait, 20);
        else Sleep(1);
        if (!port_suspenders_enter()) return 0;
        if (SuspendThread(game_thread) != (DWORD)-1) {
            CONTEXT c;
            c.ContextFlags = CONTEXT_CONTROL;
            if (GetThreadContext(game_thread, &c) && in_game_code(c.Eip) && port_interrupt_allowed() && port_interrupt_take()) {
                interrupted_eip_cell = c.Eip;
                c.Eip = (DWORD)(size_t)port_interrupt_entry;
                SetThreadContext(game_thread, &c);
                port_interrupt_count++;
            }
            ResumeThread(game_thread);
        }
        port_suspenders_leave();
    }
    return 0;
}

void port_interrupt_start(int burst_mode)
{
    burst = burst_mode;
    port_suspenders_init();
    if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &game_thread, 0, FALSE, DUPLICATE_SAME_ACCESS)) return;
    CreateThread(NULL, 0, timer, NULL, 0, NULL);
}
#else
void port_interrupt_start(int burst_mode)
{
    (void)burst_mode;
}
void port_suspenders_init(void)
{
}
void port_suspenders_stop(void)
{
}
int port_interrupt_aimed(unsigned context_eip, unsigned exception_ip, unsigned *fault_ip)
{
    (void)context_eip;
    (void)exception_ip;
    (void)fault_ip;
    return 0;
}
void port_interrupt_set_return(unsigned ip)
{
    (void)ip;
}
#endif
