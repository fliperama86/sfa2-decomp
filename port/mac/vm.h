/* The macOS port's virtual machine: one EL1 vCPU of Apple's Hypervisor framework that runs the game's C (32-bit pointers,
 * AArch64) at the PS1's own addresses. Design and reasons: README.md. Everything here is the host's side of that machine.
 *
 * One machine per process, and every call comes from the thread that called vm_create (the framework ties a vCPU to its thread);
 * only vm_interrupt may be called from another thread. A function that can fail returns 0, or -1 with one line (no line end)
 * in the caller's buffer.
 *
 * The guest's address space, as the guest sees it (the page tables are made here):
 *   0x00000000..0x001fffff  the console's copy of RAM at address 0     (alias of the RAM)
 *   0x80000000..0x801fffff  RAM                                         (VM_R | VM_W | VM_X)
 *   0xa0000000..0xa01fffff  RAM, uncached view                          (alias of the RAM)
 *   0x1f800000..0x1f800fff  the scratchpad (its first 1 KB is the PS1's) (a private page)
 *   0x1f801000..0x1f802fff  the I/O page and what follows: every access exits to the host (vm_mmio_set)
 *   0x80200000..0x87dfffff  the guest's private area: image at its base (VM_IMAGE_BASE), stacks and tables behind (vm_alloc)
 *   0x87e00000..0x87e00fff  the vectors and the routines the host calls (code, read and execute)
 *   0x87f00000..0x87ffffff  the page tables (not mapped in the guest's own view)
 */
#ifndef PORT_VM_H
#define PORT_VM_H

#include <stddef.h>
#include <stdint.h>

#define VM_RAM_BASE        0x80000000u
#define VM_RAM_SIZE        0x00200000u
#define VM_SCRATCH_BASE    0x1f800000u
#define VM_SCRATCH_PAGE    0x1000u
#define VM_IO_BASE         0x1f801000u
#define VM_IO_SIZE         0x2000u
#define VM_IMAGE_BASE      0x80200000u    /* the guest image (guest.elf) is loaded here, and may use up to VM_IMAGE_LIMIT */
#define VM_IMAGE_LIMIT     0x80600000u
#define VM_PRIVATE_LIMIT   0x87e00000u    /* vm_alloc hands out the area from VM_IMAGE_LIMIT up to here */

/* Permissions of a mapping of the guest's own view. */
enum { VM_R = 1, VM_W = 2, VM_X = 4 };

/* A routine of the host that the game calls by `hvc #number` (see guestlink.py: stubs are `hvc` and `ret`). The arguments are the
 * guest's x0 to x7, each cut to 32 bits (a pointer or an address is a guest address, an int is the low 32 bits); the result goes to x0.
 * A routine of fewer parameters is called through this type too; a routine takes what it needs. */
typedef uint64_t (*vm_hostcall_fn)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
#define VM_HOSTCALL_MAX 0xff00u           /* numbers 1 to VM_HOSTCALL_MAX - 1; 0 returns from a call into the guest */
#define VM_HOSTCALL_EXIT 0xfeffu          /* reserved: the call that VM_TRAMP_EXIT makes (a task's function returned); give it a routine with vm_hostcall_set */
#define VM_TRAMP_EXIT    0x87e00840u      /* `hvc #VM_HOSTCALL_EXIT`: the address to give a new task as its return address */

/* A routine of the host, while it runs from a hostcall: the number of the `hvc` that called it (one routine may serve many numbers). */
unsigned vm_hostcall_number(void);

/* An access to the I/O page and what follows it (VM_IO_BASE, VM_IO_SIZE): `size` is 1, 2, 4 or 8. */
typedef uint64_t (*vm_mmio_read_fn)(uint32_t address, unsigned size);
typedef void     (*vm_mmio_write_fn)(uint32_t address, unsigned size, uint64_t value);

/* What the guest did that the machine cannot go on from: an exception of its own (a vector's `hvc`), an access to unmapped memory,
 * a form of access that the exit does not describe. `what` is one line; the registers are the guest's at that point. */
struct vm_fault {
    char what[160];
    unsigned long long esr, far, elr, spsr, ipa;
    unsigned long long x[31], sp, pc;
};
/* Called with the fault. It may end the program. If it returns, the call into the guest that was running returns -1 (and the
 * guest's registers are put back as they were when the call began). */
typedef void (*vm_fault_fn)(const struct vm_fault *fault);

/* The guest ran code on a page that the host made not executable (vm_protect without VM_X): `address` is where. Return 1 when the page can be
 * run now (the host has changed it and made it executable): the guest goes on at the same instruction. 0: it is a fault. */
typedef int (*vm_exec_fault_fn)(uint32_t address);

/* Called, on the vCPU's thread, when another thread asked for an interrupt (vm_interrupt): between two guest instructions, or
 * before the guest's next one. It may call vm_call. */
typedef void (*vm_interrupt_fn)(void);

int  vm_create(char *err, size_t errsize);      /* the machine, the memory, the page tables, one vCPU; nothing runs */
void vm_destroy(void);

/* Memory. The host's pointer to a guest address, or NULL when [address, address + size) is not whole inside one of: the RAM (through
 * any of its three views), the scratchpad's page, the private area below VM_PRIVATE_LIMIT. */
void *vm_g(uint32_t address, size_t size);
/* The guest address of a host pointer into the RAM (its 0x80000000 view), the scratchpad or the private area; 0 if it is in none. */
uint32_t vm_addr_of(const void *host);
/* Give [va, va + size) the permissions `perms` (whole 4 KB pages) in the guest's own view: the private area (the image's segments, stacks), or
 * the RAM through its 0x80000000 view (a page that is not executable until the host says so). */
int   vm_protect(uint32_t va, size_t size, int perms);
/* Some bytes of the private area from VM_IMAGE_LIMIT up, never freed: the guest address, or 0 when it is full. The area is mapped
 * VM_R | VM_W. */
uint32_t vm_alloc(size_t size, size_t align);
/* After the host wrote code or changed a mapping: make the guest see it. */
void vm_sync(void);

/* The routines of the host that the guest reaches, the exit of an access to the I/O page, the fault and interrupt hooks. */
int  vm_hostcall_set(unsigned number, const char *name, vm_hostcall_fn fn);
const char *vm_hostcall_name(unsigned number);
void vm_mmio_set(vm_mmio_read_fn read, vm_mmio_write_fn write);
void vm_fault_set(vm_fault_fn fn);
void vm_interrupt_set(vm_interrupt_fn fn);
void vm_exec_fault_set(vm_exec_fault_fn fn);
void vm_interrupt(void);                         /* any thread */

/* The guest's registers (x0 to x30, 31 is the stack pointer; the program counter). Valid between exits: in a routine of the host
 * (a hostcall, an mmio handler, the interrupt hook) they are the guest's at the call. */
uint64_t vm_x(unsigned n);
void     vm_set_x(unsigned n, uint64_t value);
uint64_t vm_pc(void);
void     vm_set_pc(uint64_t pc);

/* Call the guest function at `address` with up to 8 arguments (each a 64-bit register value) and return its x0 in *result. The guest
 * runs on the stack it has now (below its stack pointer; before the first call set one with vm_set_x(31, top)). Calls nest. 0 on
 * return, -1 when the guest faulted (the fault hook ran). */
int  vm_call(uint32_t address, const uint64_t *args, unsigned nargs, uint64_t *result);

/* The state of one guest context (a task): every register. Saving and loading are for the routines that switch tasks. */
struct vm_context { uint64_t x[31], sp, pc, cpsr; };
void vm_context_save(struct vm_context *c);
void vm_context_load(const struct vm_context *c);

#endif
