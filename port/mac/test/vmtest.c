/* The host side of the machine's controls (test_vm.py): loads a guest image made by test_vm.py, runs its functions in the machine and
 * checks the results. Prints `ok NAME` or `FAIL NAME: why` for each case and a last line. Needs no disc and no game. */
#include "../elf.h"
#include "../vm.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static struct elf_image img;
static unsigned cases, failures;

static void report(const char *name, const char *why)
{
    cases++;
    if (why) {
        failures++;
        printf("FAIL %s: %s\n", name, why);
    } else printf("ok   %s\n", name);
    fflush(stdout);
}

#define EXPECT(name, cond, ...) do { if (cond) report(name, NULL); else { char m[200]; snprintf(m, sizeof m, __VA_ARGS__); report(name, m); } } while (0)

static uint32_t sym(const char *name)
{
    uint32_t a = 0;
    if (elf_find(&img, name, &a) != 0) {
        printf("FAIL the image has no symbol %s\n", name);
        exit(2);
    }
    return a;
}

static int call(const char *fn, uint64_t *result, unsigned nargs, uint64_t a0)
{
    return vm_call(sym(fn), &a0, nargs, result);
}

/* ---- the host's routines ---- */

static uint64_t test_add(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h)
{
    (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
    return a + b;
}

static uint64_t test_callback(uint64_t fn, uint64_t arg, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h)
{
    uint64_t r = 0;
    (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
    vm_call((uint32_t)fn, &arg, 1, &r);
    return r;
}

static struct vm_context ctx_a, ctx_b;
static int b_started;
static uint32_t b_stack_top;
static unsigned turn;

static uint64_t test_ctx(uint64_t which, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h)
{
    (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
    if (which == 1) {                                   /* from task A: leave it, go to B */
        vm_context_save(&ctx_a);
        ctx_a.pc = ctx_a.x[30];
        if (!b_started) {
            memset(&ctx_b, 0, sizeof ctx_b);
            ctx_b.pc = sym("g_task_b");
            ctx_b.sp = b_stack_top;
            ctx_b.cpsr = ctx_a.cpsr;
            b_started = 1;
        }
        vm_context_load(&ctx_b);
    } else {                                            /* from task B: back to A with the next turn */
        vm_context_save(&ctx_b);
        ctx_b.pc = ctx_b.x[30];
        ctx_a.x[0] = ++turn;
        vm_context_load(&ctx_a);
    }
    return 0;
}

static struct { uint32_t address; unsigned size; uint64_t value; } writes[8];
static unsigned write_count;

static uint64_t mmio_read(uint32_t address, unsigned size)
{
    if (address == 0x1f801070 && size == 4) return 0x1000;
    if (address == 0x1f801078 && size == 1) return 0x7f;
    if (address == 0x1f801090 && size == 4) return 0x11;
    if (address == 0x1f801098 && size == 1) return 0x80;
    return 0xdeadbeef;
}

static void mmio_write(uint32_t address, unsigned size, uint64_t value)
{
    if (write_count < 8) {
        writes[write_count].address = address;
        writes[write_count].size = size;
        writes[write_count].value = value;
        write_count++;
    }
}

static struct vm_fault last_fault;
static unsigned fault_count;

static void on_fault(const struct vm_fault *f)
{
    last_fault = *f;
    fault_count++;
}

static pthread_t vcpu_thread;
static uint32_t flag_address;
static int interrupt_on_vcpu_thread;

static void on_interrupt(void)
{
    interrupt_on_vcpu_thread = pthread_equal(pthread_self(), vcpu_thread);
    *(volatile uint32_t *)vm_g(flag_address, 4) = 1;
}

static void *interrupter(void *unused)
{
    (void)unused;
    usleep(50000);
    vm_interrupt();
    return NULL;
}

static void bind(const char *name, vm_hostcall_fn fn)
{
    uint32_t a = sym(name), w = 0;
    void *p = vm_g(a, 4);
    if (!p) exit(2);
    memcpy(&w, p, 4);
    if ((w & 0xffe0001f) != 0xd4000002u || ((w >> 5) & 0xffff) == 0) {
        printf("FAIL the stub %s does not begin with an hvc (word 0x%08x)\n", name, w);
        exit(2);
    }
    vm_hostcall_set((w >> 5) & 0xffff, name, fn);
}

int main(int argc, char **argv)
{
    char err[256];
    uint64_t r = 0;
    uint32_t *ram;
    struct timespec t0, t1;

    if (argc != 2) {
        fprintf(stderr, "usage: vmtest GUEST.ELF\n");
        return 2;
    }
    vcpu_thread = pthread_self();
    if (vm_create(err, sizeof err) != 0 || elf_load(argv[1], &img, err, sizeof err) != 0) {
        printf("FAIL %s\n", err);
        return 2;
    }
    bind("port_test_add", test_add);
    bind("port_test_callback", test_callback);
    bind("port_test_ctx", test_ctx);
    vm_mmio_set(mmio_read, mmio_write);
    vm_fault_set(on_fault);
    vm_interrupt_set(on_interrupt);
    vm_set_x(31, vm_alloc(0x10000, 16) + 0x10000);
    b_stack_top = vm_alloc(0x10000, 16) + 0x10000;
    flag_address = sym("g_flag");

    /* the image's segments: code runs, data is read and written */
    EXPECT("image-symbols-are-in-the-image", sym("g_sum") >= VM_IMAGE_BASE && sym("g_sum") < VM_IMAGE_LIMIT, "g_sum at 0x%x", sym("g_sum"));

    ram = vm_g(0x80100000, 32);
    ram[0] = 0x80100010; ram[1] = 5; ram[4] = 0; ram[5] = 7;
    EXPECT("a-list-in-ram-is-summed", call("g_sum", &r, 1, 0x80100000) == 0 && (int32_t)r == 12, "result %lld", (long long)r);
    EXPECT("the-same-list-is-summed-through-the-low-view", call("g_sum", &r, 1, 0x00100000) == 0 && (int32_t)r == 12, "result %lld", (long long)r);

    EXPECT("ram-views-alias", call("g_alias", &r, 0, 0) == 0 && r == 3, "result %lld", (long long)r);
    EXPECT("scratchpad-keeps-a-word", call("g_scratch", &r, 0, 0) == 0 && (uint32_t)r == 0xcafebabe, "result 0x%llx", (unsigned long long)r);
    EXPECT("the-scratchpad-is-not-in-the-ram", *(uint32_t *)vm_g(0x1f800000, 4) == 0xcafebabe && *(uint32_t *)vm_g(0x80000000, 4) != 0xcafebabe, "not separate");

    write_count = 0;
    EXPECT("io-page-reads-and-writes-exit", call("g_mmio", &r, 0, 0) == 0 && (uint32_t)r == 0x1000 + 0x7f, "result 0x%llx", (unsigned long long)r);
    EXPECT("io-page-writes-were-seen-with-size-and-value",
           write_count == 2 && writes[0].address == 0x1f801070 && writes[0].size == 4 && writes[0].value == 0x1001 &&
           writes[1].address == 0x1f801074 && writes[1].size == 2 && writes[1].value == 0xabcd,
           "%u writes; first %x/%u/%llx", write_count, writes[0].address, writes[0].size, (unsigned long long)writes[0].value);

    write_count = 0;
    EXPECT("write-back-forms-are-decoded-and-the-base-register-steps", call("g_mmio_forms", &r, 0, 0) == 0 && (uint32_t)r == 0x0904ff80 &&
           write_count == 1 && writes[0].address == 0x1f801104 && writes[0].size == 4 && writes[0].value == 0x11,
           "result 0x%llx, %u writes, first %x/%u/%llx", (unsigned long long)r, write_count, writes[0].address, writes[0].size, (unsigned long long)writes[0].value);

    EXPECT("a-hostcall-gets-its-arguments-and-returns-a-result", call("g_hostcall", &r, 0, 0) == 0 && r == 42, "result %lld", (long long)r);
    EXPECT("the-host-calls-the-guest-and-the-guest-the-host-again", call("g_nested", &r, 0, 0) == 0 && r == 1006, "result %lld", (long long)r);

    EXPECT("task-switch-through-saved-contexts", call("g_task_a", &r, 0, 0) == 0 && r == 6, "result %lld", (long long)r);

    fault_count = 0;
    EXPECT("an-unmapped-read-is-a-fault-with-its-address", call("g_read_unmapped", &r, 0, 0) == -1 && fault_count == 1 && last_fault.far == 0x40000000 &&
           (last_fault.esr >> 26) == 0x25 && strstr(last_fault.what, "guest exception") != NULL,
           "rc/faults %u far 0x%llx esr 0x%llx '%s'", fault_count, last_fault.far, last_fault.esr, last_fault.what);
    {
        uint32_t off = 0;
        const char *name = elf_symbol_at(&img, (uint32_t)last_fault.elr, &off);
        EXPECT("a-fault-names-the-function", name && strcmp(name, "g_read_unmapped") == 0, "elr 0x%llx is %s", last_fault.elr, name ? name : "nothing");
    }
    EXPECT("the-machine-goes-on-after-a-fault", call("g_sum", &r, 1, 0x80100000) == 0 && r == 12, "result %lld", (long long)r);

    fault_count = 0;
    EXPECT("an-undefined-instruction-is-a-fault", call("g_undefined", &r, 0, 0) == -1 && fault_count == 1 && (last_fault.esr >> 26) == 0x00,
           "faults %u esr 0x%llx", fault_count, last_fault.esr);
    fault_count = 0;
    EXPECT("code-cannot-be-written", call("g_write_code", &r, 0, 0) == -1 && fault_count == 1 && (last_fault.esr >> 26) == 0x25 && last_fault.far == sym("g_sum"),
           "faults %u esr 0x%llx far 0x%llx", fault_count, last_fault.esr, last_fault.far);
    fault_count = 0;
    EXPECT("data-cannot-be-run", call("g_run_data", &r, 0, 0) == -1 && fault_count == 1 && (last_fault.esr >> 26) == 0x21 && last_fault.far == sym("g_data"),
           "faults %u esr 0x%llx far 0x%llx", fault_count, last_fault.esr, last_fault.far);
    EXPECT("the-machine-goes-on-after-three-faults", call("g_sum", &r, 1, 0x80100000) == 0 && r == 12, "result %lld", (long long)r);

    {
        pthread_t th;
        pthread_create(&th, NULL, interrupter, NULL);
        EXPECT("an-interrupt-from-another-thread-runs-its-hook-between-instructions", call("g_spin", &r, 0, 0) == 0 && interrupt_on_vcpu_thread,
               "hook thread ok %d", interrupt_on_vcpu_thread);
        pthread_join(th, NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &t0);
    call("g_fib", &r, 1, 30);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    EXPECT("fib-30", r == 832040, "result %lld", (long long)r);
    printf("info fib(30) took %.0f ms\n", (double)(t1.tv_sec - t0.tv_sec) * 1000.0 + (double)(t1.tv_nsec - t0.tv_nsec) / 1e6);

    printf(failures ? "%u of %u cases behaved wrongly\n" : "all %u cases behaved as required\n", failures ? failures : cases, cases);
    vm_destroy();
    return failures ? 1 : 0;
}
