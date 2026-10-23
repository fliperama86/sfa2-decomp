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
#ifdef PORT_GUEST_VM
#include <string.h>
#include "../mac/platform.h"
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

#elif defined(PORT_GUEST_VM)

/* The macOS build: a task is a saved set of the machine's registers on a stack of its own in the guest's memory (port/mac/vm.h). The one that
 * runs has its registers in the machine; ChangeTh saves them and loads another's. */
#define TASK_STACK 0x100000u

struct slot {
    int used, fresh;
    unsigned entry;
    uint32_t stack;
    struct vm_context ctx;
};

static struct slot slots[THREAD_SLOTS];   /* slot 0 is the main thread */
static unsigned current;
static int started;

static uint64_t returned(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
    port_halt(PORT_EXIT_THREAD, "the function of a thread (0x%08x) returned; the game never lets one", slots[current].entry);
    return 0;
}

static void start(void)
{
    if (started) return;
    started = 1;
    slots[0].used = 1;
    vm_hostcall_set(VM_HOSTCALL_EXIT, "thread returned", returned);
}

static struct slot *slot_of(unsigned handle)
{
    unsigned i = handle - HANDLE_BASE;
    if (handle < HANDLE_BASE || i >= THREAD_SLOTS || !slots[i].used) return NULL;
    return &slots[i];
}

/* BIOS B0 0Eh OpenTh(pc, sp, gp): a new thread that starts at `pc` when first switched to; returns its handle, 0xff000001 and up, the lowest
 * free; -1 when none is free. */
unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp);
unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp)
{
    unsigned i;
    (void)sp;
    (void)gp;
    start();
    for (i = 1; i < THREAD_SLOTS; i++)
        if (!slots[i].used) {
            struct vm_context here;
            if (!slots[i].stack) slots[i].stack = vm_alloc(TASK_STACK, 4096);
            if (!slots[i].stack) port_halt(PORT_EXIT_OTHER, "threads: no memory for a stack");
            vm_context_save(&here);
            memset(&slots[i].ctx, 0, sizeof slots[i].ctx);
            slots[i].ctx.pc = pc;
            slots[i].ctx.sp = slots[i].stack + TASK_STACK;
            slots[i].ctx.x[30] = VM_TRAMP_EXIT;
            slots[i].ctx.cpsr = here.cpsr;
            slots[i].used = 1;
            slots[i].fresh = 1;
            slots[i].entry = pc;
            return HANDLE_BASE + i;
        }
    return 0xffffffffu;
}

/* BIOS B0 10h ChangeTh(handle): runs that thread until some thread changes back here; returns 1 then (0 at once for a handle that is not open). */
int port_h_ChangeTh(unsigned handle);
int port_h_ChangeTh(unsigned handle)
{
    struct slot *to;
    start();
    to = slot_of(handle);
    if (!to) return 0;
    if (handle - HANDLE_BASE == current) return 1;
    vm_context_save(&slots[current].ctx);
    slots[current].ctx.pc = slots[current].ctx.x[30];   /* the call returns to its caller, with 1, when this thread is run again */
    slots[current].ctx.x[0] = 1;
    current = handle - HANDLE_BASE;
    if (to->fresh) {
        to->fresh = 0;
        port_target_check("thread entry", PORT_FN_ADDR(to->entry));
    }
    vm_context_load(&to->ctx);
    return 1;
}

/* BIOS B0 0Fh CloseTh(handle): frees the slot (it is the lowest free one for the next OpenTh) and returns 1; 0 for a handle that is not open or the main
 * thread. A thread that closes itself is not run again. */
int port_h_CloseTh(unsigned handle);
int port_h_CloseTh(unsigned handle)
{
    struct slot *s;
    start();
    s = slot_of(handle);
    if (!s || handle == HANDLE_BASE) return 0;
    s->used = 0;
    return 1;
}

#elif defined(PORT_LINUX)

/* Linux: a task is a context of ucontext.h on a stack of its own (a guard page that faults, then TASK_STACK bytes), in place of a
 * Windows fiber. A task's context is saved in the task's own record at a switch, not in its slot: a task that closes itself and opens
 * another, which takes the same slot, before it switches away keeps its context out of the new task's way (as a fiber does). */
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>

#define TASK_STACK 0x100000u
#define TASK_GUARD 0x1000u

struct task {
    ucontext_t ctx;
    unsigned char *map;   /* the guard page and the stack; NULL for the start-up thread, which runs on the thread's own stack */
};

struct slot {
    int used;
    unsigned entry;
    struct task *task;
};

static struct slot slots[THREAD_SLOTS];   /* slot 0 is the main thread */
static struct task main_task;
static struct task *running;              /* the task that runs now (NULL before the first call: the start-up thread) */
static unsigned current;
static int started;
static struct task *zombie;               /* a closed task that was running; freed after the next switch */

static void free_task(struct task *t)
{
    munmap(t->map, TASK_GUARD + TASK_STACK);
    free(t);
}

static void reap(void)
{
    if (zombie) {
        struct task *t = zombie;
        zombie = NULL;
        free_task(t);
    }
}

static void start(void)
{
    if (started) return;
    started = 1;
    slots[0].used = 1;
    slots[0].task = &main_task;
    running = &main_task;
}

/* The top of the calling thread's own stack, asked once per thread. */
static unsigned long own_stack_top(void)
{
    static __thread unsigned long top;
    pthread_attr_t a;
    void *addr;
    size_t size;
    if (top) return top;
    if (pthread_getattr_np(pthread_self(), &a) != 0) return 0;
    if (pthread_attr_getstack(&a, &addr, &size) == 0) top = (unsigned long)(uintptr_t)addr + (unsigned long)size;
    pthread_attr_destroy(&a);
    return top;
}

unsigned long port_stack_top(void)
{
    if (running && running->map) return (unsigned long)(uintptr_t)(running->map + TASK_GUARD + TASK_STACK);
    return own_stack_top();
}

static void task_main(unsigned entry)
{
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

/* BIOS B0 0Eh OpenTh(pc, sp, gp): as the Windows branch. */
unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp);
unsigned port_h_OpenTh(unsigned pc, unsigned sp, unsigned gp)
{
    unsigned i;
    (void)sp;
    (void)gp;
    start();
    for (i = 1; i < THREAD_SLOTS; i++)
        if (!slots[i].used) {
            struct task *t = calloc(1, sizeof *t);
            unsigned char *map;
            if (!t) port_halt(PORT_EXIT_OTHER, "threads: out of memory");
            map = mmap(NULL, TASK_GUARD + TASK_STACK, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
            if (map == MAP_FAILED) port_halt(PORT_EXIT_OTHER, "threads: cannot make a task's stack (%s)", strerror(errno));
            t->map = map;
            if (mprotect(map, TASK_GUARD, PROT_NONE) != 0 || getcontext(&t->ctx) != 0)
                port_halt(PORT_EXIT_OTHER, "threads: cannot make a task (%s)", strerror(errno));
            t->ctx.uc_stack.ss_sp = map + TASK_GUARD;
            t->ctx.uc_stack.ss_size = TASK_STACK;
            t->ctx.uc_link = NULL;
            makecontext(&t->ctx, (void (*)(void))task_main, 1, pc);
            slots[i].used = 1;
            slots[i].entry = pc;
            slots[i].task = t;
            return HANDLE_BASE + i;
        }
    return 0xffffffffu;
}

/* BIOS B0 10h ChangeTh(handle): as the Windows branch. */
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
        struct task *self = running;
        current = handle - HANDLE_BASE;
        running = to->task;
        if (swapcontext(&self->ctx, &to->task->ctx) != 0) port_halt(PORT_EXIT_OTHER, "threads: cannot switch tasks (%s)", strerror(errno));
        current = me;
        running = self;
    }
    reap();
    return 1;
}

/* BIOS B0 0Fh CloseTh(handle): as the Windows branch; a task that closes itself is freed after the switch away from it. */
int port_h_CloseTh(unsigned handle);
int port_h_CloseTh(unsigned handle)
{
    struct slot *s;
    start();
    s = slot_of(handle);
    if (!s || handle == HANDLE_BASE) return 0;
    if (handle - HANDLE_BASE == current) zombie = s->task;
    else free_task(s->task);
    s->used = 0;
    s->task = NULL;
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
