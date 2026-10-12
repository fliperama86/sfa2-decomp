/* The macOS build's platform layer: what memory.c, interrupt.c, mirror.c and the stubs of jumps.c do for the Windows program, done for the machine
 * of vm.c. See README.md. */
#include "platform.h"

#include "../src/port.h"
#include "../src/port_tables.h"
#include "elf.h"

#include <mach-o/dyld.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *port_mac_image_path;   /* --guest FILE; by default guest.elf in the folder of the program */
static char default_image_path[4096];
unsigned port_mac_text_begin, port_mac_text_end;

static struct elf_image image;
static int trace_calls;

/* ---- the game's function names ---- */

/* The game function whose implementation is the nearest at or below `ip` ("?" if none). */
const char *port_function_at(size_t ip)
{
    uint32_t offset;
    const char *name = elf_symbol_at(&image, (uint32_t)ip, &offset);
    if (name && strncmp(name, "impl_", 5) == 0) return name + 5;
    return name ? name : "?";
}

/* ---- the faults of the guest ---- */

extern void port_debug_end(void);

static void on_fault(const struct vm_fault *f)
{
    unsigned long long pc = strncmp(f->what, "guest exception", 15) == 0 ? f->elr : f->pc;
    printf("stop: crash: %s, at 0x%08llx in %s (esr 0x%llx, address 0x%llx; x0 0x%llx x1 0x%llx x2 0x%llx x3 0x%llx, lr 0x%llx in %s)\n", f->what, pc,
           port_function_at((size_t)pc), f->esr, f->far, f->x[0] & 0xffffffffu, f->x[1] & 0xffffffffu, f->x[2] & 0xffffffffu, f->x[3] & 0xffffffffu,
           f->x[30] & 0xffffffffu, port_function_at((size_t)(f->x[30] & 0xffffffffu)));
    fflush(stdout);
    port_debug_end();
    exit(PORT_EXIT_CRASH);
}

/* ---- the I/O page ---- */

static void io_stop(const char *what, uint32_t address, const char *why)
{
    printf("stop: crash: the game used the PS1's I/O page at 0x%08x (%s) in %s %s\n", address, what, port_function_at((size_t)vm_x(30)), why);
    fflush(stdout);
    exit(PORT_EXIT_CRASH);
}

static uint64_t mmio_read(uint32_t address, unsigned size)
{
    unsigned value = 0;
    char why[160];
    if (size > 4) io_stop("read", address, "with an access of more than 4 bytes");
    if (port_iopage_read(address, size, &value, why, sizeof why) != 0) io_stop("read", address, why);
    port_iopage_served();
    return value;
}

static void mmio_write(uint32_t address, unsigned size, uint64_t value)
{
    char why[160];
    if (size > 4) io_stop("write", address, "with an access of more than 4 bytes");
    if (port_iopage_check_write(address, size, why, sizeof why) != 0) io_stop("write", address, why);
    if (port_iopage_write(address, size, (unsigned)value, why, sizeof why) != 0) io_stop("write", address, why);
    port_iopage_served();
}

/* ---- the interrupt ---- */

static volatile int timer_run;

static void on_interrupt(void)
{
    if (port_interrupt_allowed() && port_interrupt_take()) {
        extern void port_interrupt_work(void);
        port_interrupt_work();
    }
}

static void *timer(void *unused)
{
    (void)unused;
    while (timer_run) {
        usleep(1000);
        vm_interrupt();
    }
    return NULL;
}

void port_interrupt_start(int burst)
{
    pthread_t th;
    (void)burst;
    timer_run = 1;
    pthread_create(&th, NULL, timer, NULL);
    pthread_detach(th);
}

void port_suspenders_init(void) {}
void port_suspenders_stop(void) { timer_run = 0; }
void port_suspenders_check_closed(void) {}
int  port_interrupt_aimed(unsigned context_eip, unsigned exception_ip, unsigned *fault_ip) { (void)context_eip; (void)exception_ip; (void)fault_ip; return 0; }
void port_interrupt_set_return(unsigned ip) { (void)ip; }

/* ---- the hostcalls ---- */

#define HOST_MAX 4096
static struct host_entry { const void *fn; const char *name; } hosts[HOST_MAX];
static unsigned host_count;
static struct host_entry bounds[0x1000];

static uint64_t dispatch(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7)
{
    unsigned n = vm_hostcall_number();
    if (n >= PORT_MAC_STOP_BASE && n < PORT_MAC_BOUND_BASE) {
        unsigned row = n - PORT_MAC_STOP_BASE;
        extern void port_stop(unsigned returned);
        extern void port_module_stop(unsigned returned);
        if (row >= port_absent_count) port_halt(PORT_EXIT_UNKNOWN, "a stop stub of number %u, and no such row", n);
        if (trace_calls) port_trace_line("%s 0x%x 0x%x 0x%x 0x%x", port_absents[row].name, (unsigned)a0, (unsigned)a1, (unsigned)a2, (unsigned)a3);
        if (port_absents[row].image == -1) port_stop(port_absents[row].address);
        else port_module_stop(port_absents[row].address);
        return 0;
    }
    {
        const struct host_entry *e = n >= PORT_MAC_BOUND_BASE && n < PORT_MAC_BOUND_BASE + 0x1000 ? &bounds[n - PORT_MAC_BOUND_BASE] : &hosts[n - PORT_MAC_HOST_BASE];
        if (trace_calls) port_trace_line("%s 0x%x 0x%x 0x%x 0x%x", e->name, (unsigned)a0, (unsigned)a1, (unsigned)a2, (unsigned)a3);
        return ((vm_hostcall_fn)e->fn)(a0, a1, a2, a3, a4, a5, a6, a7);
    }
}

static void put_word(unsigned char *ram, unsigned address, uint32_t word)
{
    memcpy(ram + (address - VM_RAM_BASE), &word, 4);
}

void port_mac_put_branch(unsigned char *ram, unsigned address, unsigned target)
{
    int64_t delta = (int64_t)target - (int64_t)address;
    if (delta < -0x8000000 || delta >= 0x8000000 || (delta & 3))
        port_halt(PORT_EXIT_REFUSED, "a branch from 0x%08x to 0x%08x is out of reach", address, target);
    put_word(ram, address, 0x14000000u | ((uint32_t)(delta >> 2) & 0x3ffffffu));
}

void port_mac_put_hvc(unsigned char *ram, unsigned address, unsigned number)
{
    put_word(ram, address, 0xd4000002u | (number << 5));
    if (number >= PORT_MAC_STOP_BASE && number < PORT_MAC_BOUND_BASE) vm_hostcall_set(number, "stop", dispatch);
}

void port_mac_put_host(unsigned char *ram, unsigned address, const void *host, const char *name)
{
    unsigned i;
    for (i = 0; i < host_count && hosts[i].fn != host; i++) {}
    if (i == host_count) {
        if (host_count == HOST_MAX) port_halt(PORT_EXIT_REFUSED, "more than %u host routines", HOST_MAX);
        hosts[host_count].fn = host;
        hosts[host_count].name = name;
        host_count++;
        vm_hostcall_set(PORT_MAC_HOST_BASE + i, name, dispatch);
    }
    put_word(ram, address, 0xd4000002u | ((PORT_MAC_HOST_BASE + i) << 5));
}

void port_mac_sync(void)
{
    vm_sync();
}

int port_mac_stub_intact(unsigned address)
{
    const uint32_t *p = vm_g(address, 4);
    uint32_t w;
    if (!p) return 0;
    w = *p;
    if ((w & 0xffe0001fu) == 0xd4000002u) return 1;                       /* hvc */
    if ((w & 0xfc000000u) == 0x14000000u) {                               /* b: where to? it must leave the RAM */
        int32_t imm = (int32_t)((w & 0x3ffffffu) << 6) >> 6;
        uint32_t to = address + (uint32_t)(imm * 4);
        return to < VM_RAM_BASE || to - VM_RAM_BASE >= VM_RAM_SIZE;
    }
    return 0;
}

/* ---- the memory ---- */

int port_game_span(const void *p, size_t n)
{
    return ((uintptr_t)p >> 32) == 0 && vm_g((uint32_t)(uintptr_t)p, n) != NULL;
}

void port_mac_trace_calls(int on)
{
    trace_calls = on;
}

/* Called by port_map: the machine, the guest image, the hooks. */
static void task_exit(void);

int port_map(char *err, size_t errsize)
{
    uint32_t stack;
    unsigned i;

    if (!port_mac_image_path) {
        uint32_t size = sizeof default_image_path - 16;
        char *slash;
        if (_NSGetExecutablePath(default_image_path, &size) != 0 || !(slash = strrchr(default_image_path, '/'))) {
            snprintf(err, errsize, "memory: cannot find the folder of the program; give the guest image with --guest FILE");
            return -1;
        }
        strcpy(slash + 1, "guest.elf");
        port_mac_image_path = default_image_path;
    }
    if (vm_create(err, errsize) != 0) return -1;
    if (elf_load(port_mac_image_path, &image, err, errsize) != 0) return -1;
    port_mac_text_begin = image.text_begin;
    port_mac_text_end = image.text_end;
    for (i = 0; i < port_function_count; i++) {
        char name[160];
        uint32_t address = 0;
        if (!port_functions[i].impl || strlen(port_functions[i].name) > 140) continue;
        snprintf(name, sizeof name, "impl_%s", port_functions[i].name);
        if (elf_find(&image, name, &address) != 0 || address != (uint32_t)(uintptr_t)port_functions[i].impl) {
            snprintf(err, errsize, "the guest image %s is not the one this program was built with (%s)", port_mac_image_path, port_functions[i].name);
            return -1;
        }
    }
    for (i = 0; i < port_bound_count; i++) {      /* the stubs the image has for the routines that its programs' library names are bound to */
        uint32_t address = 0, word = 0;
        unsigned number;
        if (elf_find(&image, port_bounds[i].name, &address) != 0 || !vm_g(address, 4)) {
            snprintf(err, errsize, "the guest image has no stub for %s", port_bounds[i].name);
            return -1;
        }
        memcpy(&word, vm_g(address, 4), 4);
        number = (word >> 5) & 0xffff;
        if ((word & 0xffe0001fu) != 0xd4000002u || number < PORT_MAC_BOUND_BASE || number >= PORT_MAC_BOUND_BASE + 0x1000) {
            snprintf(err, errsize, "the stub of %s in the guest image is not an hvc of the programs' range (word 0x%08x)", port_bounds[i].name, word);
            return -1;
        }
        bounds[number - PORT_MAC_BOUND_BASE].fn = port_bounds[i].host;
        bounds[number - PORT_MAC_BOUND_BASE].name = port_bounds[i].name;
        vm_hostcall_set(number, port_bounds[i].name, dispatch);
    }
    stack = vm_alloc(0x100000, 4096);
    if (!stack) {
        snprintf(err, errsize, "memory: no stack for the game");
        return -1;
    }
    vm_set_x(31, stack + 0x100000);
    vm_mmio_set(mmio_read, mmio_write);
    vm_fault_set(on_fault);
    vm_interrupt_set(on_interrupt);
    (void)task_exit;
    return 0;
}

void port_unmap(void)
{
    vm_destroy();
}

static void task_exit(void) {}
