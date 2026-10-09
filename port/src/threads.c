/* The BIOS threads (OpenTh, CloseTh, ChangeTh) and GetGp.
 *
 * The game uses them as cooperative tasks (read from its C: three task slots,
 * at most three live threads besides the main one, a switch only at an
 * explicit ChangeTh). On Windows each is a fiber: the start-up thread becomes a fiber
 * and is handle 0xff000000; OpenTh makes a fiber that calls the entry
 * and returns 0xff000001 and up, the lowest free slot as the BIOS hands
 * them out; ChangeTh switches. There are four slots, the BIOS's default
 * (the main thread and three more): the game's C writes the control blocks 1 to 3.
 * The thread's stack pointer and gp are not used: the fiber has its own stack and
 * the game's C does not read gp (all its units are built with -G0). */
#include "port.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define THREAD_SLOTS  4
#define HANDLE_BASE   0xff000000u
#define FIBER_RESERVE 0x100000u
#define FIBER_COMMIT  0x10000u

static unsigned gp_value;

void port_set_gp(unsigned gp)
{
    gp_value = gp;
}

/* From the entry code as loaded: the first `lui $gp, hi` among the first 64
 * instructions and the `addiu $gp, $gp, lo` after it. */
int port_entry_gp(const unsigned char *ram, unsigned pc0, unsigned *gp)
{
    unsigned i, j;

    for (i = 0; i < 64; i++) {
        const unsigned char *p = ram + (pc0 - PORT_RAM_BASE) + 4 * i;
        unsigned w = p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24;
        if ((w >> 16) != 0x3c1c) continue;          /* lui $28, imm */
        for (j = i + 1; j < i + 8 && j < 64; j++) {
            const unsigned char *q = ram + (pc0 - PORT_RAM_BASE) + 4 * j;
            unsigned v = q[0] | q[1] << 8 | q[2] << 16 | (unsigned)q[3] << 24;
            if ((v >> 16) == 0x279c) {              /* addiu $28, $28, imm */
                *gp = (w << 16) + (unsigned)(int)(short)(v & 0xffffu);
                return 0;
            }
        }
    }
    return -1;
}

/* The listing at 0x8015789c: `move v0, gp; jr ra`. gp is what the entry loaded. */
unsigned port_h_GetGp(void);
unsigned port_h_GetGp(void)
{
    return gp_value;
}

#ifdef _WIN32

struct slot {
    int used;
    unsigned entry;
    void *fiber;
};

static struct slot slots[THREAD_SLOTS];   /* slot 0 is the main thread */
static unsigned current;
static int started;
static void *zombie;                      /* a fiber of a closed thread that was running; deleted after the next switch */

static void reap(void)
{
    if (zombie) {
        void *f = zombie;
        zombie = NULL;
        DeleteFiber(f);
    }
}

static void start(void)
{
    if (started) return;
    started = 1;
    slots[0].used = 1;
    slots[0].fiber = ConvertThreadToFiber(NULL);
    if (!slots[0].fiber) port_halt(PORT_EXIT_OTHER, "threads: cannot make the start-up thread a fiber (system error %lu)", (unsigned long)GetLastError());
}

static VOID CALLBACK fiber_main(void *param)
{
    unsigned entry = (unsigned)(size_t)param;
    reap();
    port_target_check("thread entry", (const void *)(size_t)entry);
    ((void (*)(void))(size_t)entry)();
    port_halt(PORT_EXIT_THREAD, "the function of a thread (0x%08x) returned; the game never lets one", entry);
}

static struct slot *slot_of(unsigned handle)
{
    unsigned i = handle - HANDLE_BASE;
    if (handle < HANDLE_BASE || i >= THREAD_SLOTS || !slots[i].used) return NULL;
    return &slots[i];
}

/* BIOS B0 0Eh OpenTh(pc, sp, gp): a new thread that starts at `pc` when first
 * switched to; returns its handle, 0xff000001 and up, the lowest free; -1 when
 * none is free. (The listing: a B0 stub, call 0x0e.) */
unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp);
unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp)
{
    unsigned i;
    (void)sp;
    (void)gp;
    start();
    for (i = 1; i < THREAD_SLOTS; i++)
        if (!slots[i].used) {
            slots[i].fiber = CreateFiberEx(FIBER_COMMIT, FIBER_RESERVE, 0, fiber_main, (void *)(size_t)pc);
            if (!slots[i].fiber) port_halt(PORT_EXIT_OTHER, "threads: cannot make a fiber (system error %lu)", (unsigned long)GetLastError());
            slots[i].used = 1;
            slots[i].entry = pc;
            return HANDLE_BASE + i;
        }
    return 0xffffffffu;
}

/* BIOS B0 10h ChangeTh(handle): runs that thread until some thread changes
 * back here; returns 1 then (0 at once for a handle that is not open). */
int port_h_ChangeTh(unsigned handle);
int port_h_ChangeTh(unsigned handle)
{
    struct slot *to;
    start();
    to = slot_of(handle);
    if (!to) return 0;
    if (handle - HANDLE_BASE == current) return 1;
    {
        unsigned me = current;
        current = handle - HANDLE_BASE;
        SwitchToFiber(to->fiber);
        current = me;
    }
    reap();
    return 1;
}

/* BIOS B0 0Fh CloseTh(handle): frees the slot (it is the lowest free one for
 * the next OpenTh) and returns 1; 0 for a handle that is not open or the main
 * thread. A thread that closes itself is not run again; its fiber is deleted
 * after the switch away from it, since a fiber cannot delete itself. */
int port_h_CloseTh(unsigned handle);
int port_h_CloseTh(unsigned handle)
{
    struct slot *s;
    start();
    s = slot_of(handle);
    if (!s || handle == HANDLE_BASE) return 0;
    if (handle - HANDLE_BASE == current) zombie = s->fiber;
    else DeleteFiber(s->fiber);
    s->used = 0;
    s->fiber = NULL;
    return 1;
}

#else

unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp);
unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp)
{
    (void)pc; (void)sp; (void)gp;
    port_halt(PORT_EXIT_OTHER, "threads are written for Windows fibers only");
    return 0;
}
int port_h_ChangeTh(unsigned handle);
int port_h_ChangeTh(unsigned handle)
{
    (void)handle;
    return 0;
}
int port_h_CloseTh(unsigned handle);
int port_h_CloseTh(unsigned handle)
{
    (void)handle;
    return 0;
}

#endif

const struct port_library port_thread_library[] = {
    { "func_8015760c", (void *)port_h_OpenTh, NULL },
    { "func_801577bc", (void *)port_h_CloseTh, NULL },
    { "func_801577ec", (void *)port_h_ChangeTh, NULL },
    { "func_8015789c", (void *)port_h_GetGp, NULL },
    { NULL, NULL, NULL }
};
