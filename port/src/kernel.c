/* The BIOS and the interrupt library: events, critical sections, root counters,
 * the callbacks, VSync, the pads' buffers, and the frame clock (port_tick)
 * that stands in for the vblank interrupt.
 *
 * There are no interrupts here. The game's waits and polls come to
 * port_tick, which does one vblank when 1/60 s has passed since the last.
 *
 * The source of each routine's behaviour is in the comment above it: the
 * listing of the game's library (the resident code, addresses given), PSY-Q's
 * documented behaviour, or the BIOS call table (PSX-SPX). */
#include "port.h"

#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

/* ---- time ---- */

#define FRAME_US       16667ull    /* one frame: 1/60 s (NTSC) */
#define LINES_PER_SEC  15750ull    /* horizontal retraces a second at 60 frames of 262.5 lines */
#define SYSCLK8_HZ     4233600ull  /* the system clock (33.8688 MHz) divided by 8: root counter 2 */

static unsigned long long now_us(void)
{
#ifdef _WIN32
    static LARGE_INTEGER freq;
    LARGE_INTEGER c;
    if (!freq.QuadPart) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&c);
    return (unsigned long long)c.QuadPart * 1000000ull / (unsigned long long)freq.QuadPart;
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (unsigned long long)t.tv_sec * 1000000ull + (unsigned long long)t.tv_nsec / 1000ull;
#endif
}

static void sleep_us(unsigned long long us)
{
#ifdef _WIN32
    static HANDLE timer;
    static int tried;
    if (!tried) {
        tried = 1;
        /* CREATE_WAITABLE_TIMER_HIGH_RESOLUTION (0x2): a timer that is exact to well under a millisecond, where the system has it */
        timer = CreateWaitableTimerExW(NULL, NULL, 0x2, TIMER_ALL_ACCESS);
    }
    if (us >= 1000 && timer) {
        LARGE_INTEGER due;
        due.QuadPart = -(long long)(us * 10ull);
        if (SetWaitableTimer(timer, &due, 0, NULL, NULL, 0)) {
            WaitForSingleObject(timer, 20);
            return;
        }
    }
    Sleep(us >= 1000 ? 1 : 0);
#else
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = (long)(us * 1000ull);
    nanosleep(&t, NULL);
#endif
}

/* ---- state ---- */

static int callbacks_active;     /* ResetCallback has been called and StopCallback has not */
static int interrupts_enabled = 1; /* the processor's interrupt enable: EnterCriticalSection clears it */
static int irq0_enabled;         /* the vblank interrupt is unmasked (I_MASK bit 0) */
static int vblank_pending;       /* a vblank came while interrupts were disabled */
static int vblank_in_progress;
static unsigned total_frames;    /* every vblank since the start (for the debug aids; vcount is zeroed by ResetCallback) */
static unsigned vcount;          /* vblanks since the start (Vcount of the library, which ResetCallback zeroes) */
static unsigned vsync_prev;      /* Vcount when VSync last returned */
static unsigned long long vsync_prev_us;
volatile int port_handler_depth;  /* > 0 while a handler of the game runs (the PS1 does not nest interrupts) */
static unsigned long long clock_start_us;   /* the one clock: vblank k is due at clock_start + k / 60 s */
static volatile long vblanks_taken;         /* how many vblanks have been taken (delivered, or skipped when far behind) */
static void (*vsync_handler)(void);
static void (*irq_handler[11])(void);
static int irq_dummy;            /* ResetCallback returns the address of this: Sony's returns its interrupt environment */

/* ---- events ---- */

#define EVENT_SLOTS 16
#define EV_FREE     0x0000
#define EV_DISABLED 0x1000
#define EV_ENABLED  0x2000
#define EV_READY    0x4000
#define EV_MD_CALL  0x1000      /* EvMdINTR: run the handler */

struct event {
    unsigned event_class, spec, mode;
    void (*handler)(void);
    unsigned status;
};
static struct event events[EVENT_SLOTS];

static struct event *event_of(unsigned descriptor)
{
    unsigned i = descriptor & 0xffffu;
    if ((descriptor & 0xffff0000u) != 0xf1000000u || i >= EVENT_SLOTS || events[i].status == EV_FREE) return NULL;
    return &events[i];
}

/* BIOS B0 08h OpenEvent(class, spec, mode, func): the lowest free slot, disabled; the descriptor is 0xf1000000 plus the slot. 0xffffffff if none is free. */
unsigned port_h_OpenEvent(unsigned event_class, unsigned spec, unsigned mode, void (*handler)(void));
unsigned port_h_OpenEvent(unsigned event_class, unsigned spec, unsigned mode, void (*handler)(void))
{
    unsigned i;
    for (i = 0; i < EVENT_SLOTS; i++)
        if (events[i].status == EV_FREE) {
            events[i].event_class = event_class;
            events[i].spec = spec;
            events[i].mode = mode;
            events[i].handler = handler;
            events[i].status = EV_DISABLED;
            return 0xf1000000u | i;
        }
    return 0xffffffffu;
}

/* BIOS B0 0Ch EnableEvent(ev): status becomes enabled; returns 1. */
int port_h_EnableEvent(unsigned ev);
int port_h_EnableEvent(unsigned ev)
{
    struct event *e = event_of(ev);
    if (!e) return 0;
    e->status = EV_ENABLED;
    return 1;
}

/* BIOS B0 0Dh DisableEvent(ev): status becomes disabled; returns 1. */
int port_h_DisableEvent(unsigned ev);
int port_h_DisableEvent(unsigned ev)
{
    struct event *e = event_of(ev);
    if (!e) return 0;
    e->status = EV_DISABLED;
    return 1;
}

/* BIOS B0 09h CloseEvent(ev): frees the slot; returns 1. */
int port_h_CloseEvent(unsigned ev);
int port_h_CloseEvent(unsigned ev)
{
    struct event *e = event_of(ev);
    if (!e) return 0;
    e->status = EV_FREE;
    return 1;
}

/* BIOS B0 0Bh TestEvent(ev): 1 and back to enabled if the event was delivered
 * (ready), else 0. The game polls it in loops, so it comes to the frame clock. */
int port_h_TestEvent(unsigned ev);
int port_h_TestEvent(unsigned ev)
{
    struct event *e;
    port_tick();
    e = event_of(ev);
    if (e && e->status == EV_READY) {
        e->status = EV_ENABLED;
        return 1;
    }
    return 0;
}

/* BIOS B0 07h DeliverEvent(class, spec): every enabled event of that class and
 * spec has its handler called (mode 0x1000) or is marked ready (other modes). */
void port_deliver_event(unsigned event_class, unsigned spec)
{
    unsigned i;
    for (i = 0; i < EVENT_SLOTS; i++) {
        struct event *e = &events[i];
        if (e->status != EV_ENABLED || e->event_class != event_class || e->spec != spec) continue;
        if (e->mode == EV_MD_CALL && e->handler) {
            port_handler_depth++;
            e->handler();
            port_handler_depth--;
        }
        else e->status = EV_READY;
    }
}

/* ---- critical sections ---- */

/* The BIOS's syscall 1: interrupts off; returns 1 if they were on, 0 if not
 * (the processor's status register told the caller). The listing: a `syscall`
 * instruction at 0x801575dc. */
int port_h_EnterCriticalSection(void);
int port_h_EnterCriticalSection(void)
{
    int was = interrupts_enabled;
    interrupts_enabled = 0;
    return was;
}

/* The BIOS's syscall 2: interrupts on. The listing: a `syscall` at 0x8015786c.
 * A vblank that came meanwhile is delivered at the next tick. */
void port_h_ExitCriticalSection(void);
void port_h_ExitCriticalSection(void)
{
    interrupts_enabled = 1;
}

/* ---- root counters ---- */

/* Counters 0 to 2 as the game's library has them (the listing at 0x8015764c to
 * 0x80157784: the index is the low half of the spec, 3 and above are refused,
 * 0 is returned, and the vblank counter 3 is only an interrupt mask bit). Only
 * counter 1 (horizontal retraces, the one the game resets and reads every
 * frame) and 2 (the system clock / 8) are kept; both count from their last
 * reset, on the frame clock. */
static unsigned long long counter_base_us[3];

static unsigned counter_rate(unsigned long long i)
{
    return (unsigned)(i == 1 ? LINES_PER_SEC : SYSCLK8_HZ);
}

/* The listing at 0x8015764c: index >= 3 returns 0 and changes nothing; else
 * the counter's mode is cleared, its target is set to `target`, and a mode word
 * is built from `mode`; returns 1. The mode and target only steer the
 * counter's interrupt, which is not modelled, so any index below 3 stops here. */
int port_h_SetRCnt(unsigned spec, int target, int mode);
int port_h_SetRCnt(unsigned spec, int target, int mode)
{
    unsigned i = spec & 0xffffu;
    if (i >= 3) return 0;
    port_halt(PORT_EXIT_OTHER, "SetRCnt on root counter %u (target 0x%x, mode 0x%x) is not served: the counter's interrupt is not modelled", i, (unsigned)target, (unsigned)mode);
    return 1;
}

/* The listing at 0x80157720: sets the counter's bit in the interrupt mask
 * (table 0x10, 0x20, 0x40, 0x01 for counters 0 to 3: so 3 is IRQ 0, the
 * vblank); the return value is 1 for counters 0 to 2 and 0 for 3. */
int port_h_StartRCnt(unsigned spec);
int port_h_StartRCnt(unsigned spec)
{
    unsigned i = spec & 0xffffu;
    if (i == 3) irq0_enabled = 1;
    else if (i < 3) port_halt(PORT_EXIT_OTHER, "StartRCnt on root counter %u is not served: the counter's interrupt is not modelled", i);
    return i < 3;
}

/* The listing at 0x80157784: index < 3 sets the counter to 0 and returns 1;
 * else returns 0 and does nothing. The counter then counts from this moment. */
int port_h_ResetRCnt(unsigned spec);
int port_h_ResetRCnt(unsigned spec)
{
    unsigned i = spec & 0xffffu;
    if (i >= 3) return 0;
    if (i == 0) port_halt(PORT_EXIT_OTHER, "ResetRCnt on root counter 0 (the dot clock) is not served");
    counter_base_us[i] = now_us();
    return 1;
}

/* The listing at 0x801576e8: index >= 3 returns 0; else the counter's 16-bit
 * value. Counter 1 counts horizontal retraces, counter 2 the system clock / 8
 * (PSX-SPX, the default source of each); both from their last reset. A game
 * waits on it in loops, so it comes to the frame clock. */
int port_h_GetRCnt(unsigned spec);
int port_h_GetRCnt(unsigned spec)
{
    unsigned i = spec & 0xffffu;
    port_tick();
    if (i >= 3) return 0;
    if (i == 0) port_halt(PORT_EXIT_OTHER, "GetRCnt on root counter 0 (the dot clock) is not served");
    return (int)(((now_us() - counter_base_us[i]) * counter_rate(i) / 1000000ull) & 0xffffull);
}

/* ---- the pads (InitPAD / StartPAD) ---- */

static unsigned char *pad_buffer[2];
static int pad_len[2];
static int pad_started;

/* BIOS B0 12h InitPAD(buf1, len1, buf2, len2): the BIOS reads the pads every
 * vblank into these two buffers. Returns 1. */
int port_h_InitPAD(void *buf1, int len1, void *buf2, int len2);
int port_h_InitPAD(void *buf1, int len1, void *buf2, int len2)
{
    pad_buffer[0] = buf1;
    pad_len[0] = len1;
    pad_buffer[1] = buf2;
    pad_len[1] = len2;
    return 1;
}

/* BIOS B0 13h StartPAD: starts that reading; returns 1. */
int port_h_StartPAD(void);
int port_h_StartPAD(void)
{
    pad_started = 1;
    return 1;
}

/* The listing: a B0 stub for 0x5b ChangeClearPAD(int); PSY-Q's documentation says it
 * sets whether the pad reading clears at vblank; nothing to do here. */
int port_h_ChangeClearPAD(int flag);
int port_h_ChangeClearPAD(int flag)
{
    (void)flag;
    return 0;
}

/* Port 1 is a digital pad with no button pressed, in the BIOS's documented format (status 0, kind 0x41, two
 * bytes of buttons, a 0 bit for a pressed button). Port 2 is empty: 0xff, the BIOS's "no controller". */
static void pads_fill(void)
{
    if (!pad_started) return;
    if (pad_buffer[0] && pad_len[0] > 0) {
        memset(pad_buffer[0], 0, (size_t)pad_len[0]);
        if (pad_len[0] > 1) pad_buffer[0][1] = 0x41;
        if (pad_len[0] > 3) pad_buffer[0][2] = pad_buffer[0][3] = 0xff;
    }
    if (pad_buffer[1] && pad_len[1] > 0) memset(pad_buffer[1], 0xff, (size_t)pad_len[1]);
}

/* ---- callbacks ---- */

/* PSY-Q's documented ResetCallback (libetc intr.c, the same code is in the
 * game's library at 0x8015efc0, its start-up path 0x8015f158): the first call
 * starts the interrupt library: Vcount is zeroed, the library's vblank handler
 * is installed (the vblank interrupt is unmasked), interrupts are enabled
 * (ExitCriticalSection), and a non-null value is returned; a second call while
 * it is running returns 0 and changes nothing. */
void port_callbacks_reset(void)
{
    if (callbacks_active) return;
    callbacks_active = 1;
    vcount = 0;
    vsync_prev = 0;
    vsync_handler = NULL;
    memset(irq_handler, 0, sizeof irq_handler);
    irq0_enabled = 1;
    interrupts_enabled = 1;
}

void *port_h_ResetCallback(void);
void *port_h_ResetCallback(void)
{
    int was = callbacks_active;
    port_callbacks_reset();
    return was ? NULL : &irq_dummy;
}

/* PSY-Q's documented StopCallback (0x8015f0b4; intr.c stopIntr): if running,
 * it disables interrupts (EnterCriticalSection), masks every interrupt source
 * and takes the library's handlers off. Interrupts stay disabled until the
 * game enables them again (ExitCriticalSection, or ResetCallback). */
void port_h_StopCallback(void);
void port_h_StopCallback(void)
{
    if (!callbacks_active) return;
    interrupts_enabled = 0;
    irq0_enabled = 0;
    callbacks_active = 0;
}

/* PSY-Q's documented InterruptCallback(irq, f) (0x8015eff0; intr.c setIntr):
 * sets the handler of interrupt `irq` (0 to 10) and returns the previous one. Only the
 * vblank (irq 0) is ever raised here, in place of the library's own handler. */
void *port_h_InterruptCallback(int irq, void (*handler)(void));
void *port_h_InterruptCallback(int irq, void (*handler)(void))
{
    void (*previous)(void);
    if (irq < 0 || irq > 10 || !callbacks_active) return NULL;
    previous = irq_handler[irq];
    irq_handler[irq] = handler;
    return (void *)previous;
}

/* PSY-Q's documented VSyncCallback(f) (0x8015f050): f is called at every
 * vblank, from the library's vblank handler; NULL removes it. */
void port_h_VSyncCallback(void (*handler)(void));
void port_h_VSyncCallback(void (*handler)(void))
{
    vsync_handler = handler;
}

/* ---- the frame clock ---- */

/* The vblank: bumps Vcount, fills the pads, then (interrupts enabled) the
 * BIOS's root counter 3 event, then the library's vblank handler. Disabled
 * interrupts hold the delivery back to the next tick. */
static void deliver_vblank(void)
{
    port_handler_depth++;
    if (irq0_enabled) port_deliver_event(0xf2000003u, 2);
    if (callbacks_active) {
        if (irq_handler[0]) irq_handler[0]();
        else if (vsync_handler) vsync_handler();
    }
    port_handler_depth--;
}

unsigned port_frames(void)
{
    return total_frames;
}

/* The one clock. A vblank is due when 1/60 s has passed since the previous one's slot; it is taken exactly once,
 * by whichever comes first: a library routine that ticks (port_tick) or the timer thread interrupting the game
 * (interrupt.c). Both take it with this same step. Far behind (the host was stopped): the lag is one vblank,
 * not a storm of them. */
static long vblanks_due(unsigned long long now)
{
    return (long)((now - clock_start_us) / FRAME_US);
}

static int take_vblank(unsigned long long now)
{
    long due = vblanks_due(now);
    for (;;) {
        long t = vblanks_taken, n;
        if (t >= due) return 0;
        n = due - t > 4 ? due : t + 1;
#ifdef _WIN32
        if (InterlockedCompareExchange(&vblanks_taken, n, t) == t) return 1;
#else
        if (__sync_bool_compare_and_swap(&vblanks_taken, t, n)) return 1;
#endif
    }
}

/* What one vblank does, after it was taken. Called on the game's thread: from port_tick, or from the
 * interruption routine of interrupt.c. */
static void run_vblank(void)
{
    vcount++;
    total_frames++;
    pads_fill();
    vblank_pending = 1;
    vblank_in_progress = 1;
    if (interrupts_enabled) {
        vblank_pending = 0;
        deliver_vblank();
    }
    vblank_in_progress = 0;
}

/* The timer thread's view (interrupt.c), read while the game's thread is suspended: may a vblank interrupt it now?
 * Not inside a handler, not with interrupts disabled (EnterCriticalSection), not while a vblank is being done. */
int port_interrupt_allowed(void)
{
    return port_handler_depth == 0 && interrupts_enabled && !vblank_in_progress;
}

/* The timer thread's step: with the game's thread suspended at a place where an interruption is allowed, take
 * the vblank if one is due. 1: taken (the caller redirects the thread to the interruption routine). */
int port_interrupt_take(void)
{
    return take_vblank(now_us());
}

/* The interruption routine's C part (interrupt.c's assembly calls it on the game's thread). */
void port_interrupt_work(void);
void port_interrupt_work(void)
{
    run_vblank();
}

void port_clock_start(void)
{
    clock_start_us = now_us();
}

void port_tick(void)
{
    unsigned long long now;

    if (port_handler_depth || vblank_in_progress) return;
    now = now_us();
    if (vblanks_taken >= vblanks_due(now)) {
        unsigned long long next = clock_start_us + (unsigned long long)(vblanks_taken + 1) * FRAME_US;
        unsigned long long left = next > now ? next - now : 0;
        sleep_us(left > 2000 ? 2000 : left);
        now = now_us();
    }
    if (take_vblank(now)) {
        run_vblank();
    } else if (vblank_pending && interrupts_enabled) {
        vblank_pending = 0;
        vblank_in_progress = 1;
        deliver_vblank();
        vblank_in_progress = 0;
    }
}

/* VSync as the game's library has it (libetc vsync.c, listing 0x8015fb30):
 * mode < 0 returns Vcount; mode 1 returns the horizontal retraces since the
 * last VSync returned; otherwise it waits until Vcount reaches the count at the last return plus
 * `mode` minus 1 (nothing for mode 0), then for one more vblank, and returns the retraces that had passed
 * at entry. Sony's, while the interrupt library is not running, gives up after
 * a count of iterations and prints "VSync: timeout"; here it waits for the vblank
 * in every state (a judgment: the game's own purpose for the call). */
int port_h_VSync(int mode);
int port_h_VSync(int mode)
{
    unsigned long long entered = now_us();
    int ret = (int)(((entered - vsync_prev_us) * LINES_PER_SEC / 1000000ull) & 0xffffull);
    unsigned first, target;

    if (mode < 0) {
        port_tick();
        return (int)vcount;
    }
    if (mode == 1) {
        port_tick();
        return ret;
    }
    first = vsync_prev + (mode > 0 ? (unsigned)(mode - 1) : 0u);
    while ((int)(vcount - first) < 0) port_tick();
    target = vcount + 1;
    while ((int)(vcount - target) < 0) port_tick();
    vsync_prev = vcount;
    vsync_prev_us = now_us();
    return ret;
}

/* ---- the table ---- */

const struct port_library port_kernel_library[] = {
    { "OpenEvent", (void *)port_h_OpenEvent, NULL },
    { "EnableEvent", (void *)port_h_EnableEvent, NULL },
    { "DisableEvent", (void *)port_h_DisableEvent, NULL },
    { "CloseEvent", (void *)port_h_CloseEvent, NULL },
    { "TestEvent", (void *)port_h_TestEvent, NULL },
    { "EnterCriticalSection", (void *)port_h_EnterCriticalSection, NULL },
    { "ExitCriticalSection", (void *)port_h_ExitCriticalSection, NULL },
    { "func_8015764c", (void *)port_h_SetRCnt, NULL },
    { "GetRCnt", (void *)port_h_GetRCnt, NULL },
    { "func_80157720", (void *)port_h_StartRCnt, NULL },
    { "func_80157784", (void *)port_h_ResetRCnt, NULL },
    { "func_8015762c", (void *)port_h_InitPAD, NULL },
    { "func_801577dc", (void *)port_h_StartPAD, NULL },
    { "ChangeClearPAD", (void *)port_h_ChangeClearPAD, NULL },
    { "ResetCallback", (void *)port_h_ResetCallback, NULL },
    { "StopCallback", (void *)port_h_StopCallback, NULL },
    { "InterruptCallback", (void *)port_h_InterruptCallback, NULL },
    { "VSyncCallback", (void *)port_h_VSyncCallback, NULL },
    { "VSync", (void *)port_h_VSync, NULL },
    { NULL, NULL, NULL }
};
