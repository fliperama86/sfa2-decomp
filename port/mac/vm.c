/* The macOS port's virtual machine. See vm.h and README.md. */
#include "vm.h"

#include <Hypervisor/Hypervisor.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define SCRATCH_IPA     0x90000000u
#define SCRATCH_ALLOC   0x4000u              /* the framework's granule */
#define VECTORS         0x87e00000u
#define TRAMP_RET       0x87e00800u          /* hvc #0 */
#define TRAMP_SYNC      0x87e00810u          /* ic iallu; dsb ish; tlbi vmalle1; dsb ish; isb; ret */
#define TRAMP_END       0x87e01000u
#define POOL_IPA        0x87f00000u          /* the page tables */
#define POOL_SIZE       0x00100000u
#define PRIVATE_IPA_END 0x88000000u
#define CPSR_EL1H       0x3c5u               /* EL1 with its own stack pointer, interrupts masked */

/* Page table descriptors, 4 KB granule. */
#define D_TABLE   3ull
#define D_PAGE    3ull
#define D_AF      (1ull << 10)
#define D_SH_IS   (3ull << 8)
#define D_RO      (1ull << 7)                /* AP[2]: read only at EL1 */
#define D_PXN     (1ull << 53)
#define D_UXN     (1ull << 54)
#define D_ATTR(n) ((uint64_t)(n) << 2)
#define ATTR_NORMAL 0
#define ATTR_DEVICE 1

static struct {
    int created, started;
    hv_vcpu_t cpu;
    hv_vcpu_exit_t *exit;
    uint8_t *ram, *scratch, *priv;
    uint32_t brk;
    uint32_t pool_next;
    uint64_t *l1;
    struct { const char *name; vm_hostcall_fn fn; } calls[VM_HOSTCALL_MAX];
    vm_mmio_read_fn mmio_read;
    vm_mmio_write_fn mmio_write;
    vm_fault_fn fault_fn;
    vm_interrupt_fn interrupt_fn;
    int redirected;                          /* a routine of the host set the guest's pc or loaded a context */
    struct vm_fault fault;
} V;

#define PRIV_SIZE (PRIVATE_IPA_END - VM_IMAGE_BASE)

static uint8_t *priv_at(uint32_t ipa)
{
    return V.priv + (ipa - VM_IMAGE_BASE);
}

/* ---- the page tables ---- */

static uint64_t *table_new(uint64_t *entry)
{
    uint32_t ipa;
    uint64_t *t;
    if (V.pool_next + 0x1000 > POOL_IPA + POOL_SIZE) return NULL;
    ipa = V.pool_next;
    V.pool_next += 0x1000;
    t = (uint64_t *)priv_at(ipa);
    memset(t, 0, 0x1000);
    *entry = ipa | D_TABLE;
    return t;
}

static uint64_t *table_of(uint64_t entry)
{
    return (uint64_t *)priv_at((uint32_t)(entry & 0xfffff000u));
}

/* The leaf entry of the 4 KB page at `va`, made if `make`. */
static uint64_t *leaf(uint32_t va, int make)
{
    uint64_t *l2, *l3, *e = &V.l1[va >> 30];
    if (!(*e & 1)) {
        if (!make || !(l2 = table_new(e))) return NULL;
    } else l2 = table_of(*e);
    e = &l2[(va >> 21) & 0x1ff];
    if (!(*e & 1)) {
        if (!make || !(l3 = table_new(e))) return NULL;
    } else l3 = table_of(*e);
    return &l3[(va >> 12) & 0x1ff];
}

static uint64_t page_bits(int perms, int attr)
{
    uint64_t d = D_PAGE | D_AF | D_SH_IS | D_ATTR(attr) | D_UXN;
    if (!(perms & VM_W)) d |= D_RO;
    if (!(perms & VM_X)) d |= D_PXN;
    return d;
}

/* Map [va, va + size) to [ipa, ...) with `perms`; whole pages. */
static int map(uint32_t va, uint32_t ipa, size_t size, int perms, int attr)
{
    size_t i;
    for (i = 0; i < size; i += 0x1000) {
        uint64_t *e = leaf(va + (uint32_t)i, 1);
        if (!e) return -1;
        *e = ((uint64_t)(ipa + i) & 0xfffffffff000ull) | page_bits(perms, attr);
    }
    return 0;
}

int vm_protect(uint32_t va, size_t size, int perms)
{
    size_t i;
    if (va < VM_IMAGE_BASE || va % 0x1000 || size % 0x1000 || va + size > VM_PRIVATE_LIMIT || va + size < va) return -1;
    for (i = 0; i < size; i += 0x1000) {
        uint64_t *e = leaf(va + (uint32_t)i, 0);
        if (!e || !(*e & 1)) return -1;
        *e = (*e & 0xfffffffff000ull) | page_bits(perms, ATTR_NORMAL);
    }
    if (V.started) vm_sync();
    return 0;
}

/* ---- memory ---- */

void *vm_g(uint32_t a, size_t n)
{
    uint64_t end = (uint64_t)a + n;
    uint32_t off = a & 0x1fffffffu;
    uint32_t region = a >> 29;
    if ((region == 0 || region == 4 || region == 5) && off < VM_RAM_SIZE) return end - (a - off) <= VM_RAM_SIZE ? V.ram + off : NULL;
    if (a >= VM_SCRATCH_BASE && end <= VM_SCRATCH_BASE + VM_SCRATCH_PAGE) return V.scratch + (a - VM_SCRATCH_BASE);
    if (a >= VM_IMAGE_BASE && end <= VM_PRIVATE_LIMIT) return priv_at(a);
    return NULL;
}

uint32_t vm_addr_of(const void *host)
{
    const uint8_t *p = host;
    if (p >= V.ram && p < V.ram + VM_RAM_SIZE) return VM_RAM_BASE + (uint32_t)(p - V.ram);
    if (p >= V.scratch && p < V.scratch + VM_SCRATCH_PAGE) return VM_SCRATCH_BASE + (uint32_t)(p - V.scratch);
    if (p >= V.priv && p < V.priv + (VM_PRIVATE_LIMIT - VM_IMAGE_BASE)) return VM_IMAGE_BASE + (uint32_t)(p - V.priv);
    return 0;
}

uint32_t vm_alloc(size_t size, size_t align)
{
    uint64_t at;
    if (align < 16) align = 16;
    at = ((uint64_t)V.brk + align - 1) & ~((uint64_t)align - 1);
    if (at + size > VM_PRIVATE_LIMIT) return 0;
    V.brk = (uint32_t)(at + size);
    return (uint32_t)at;
}

/* ---- registers ---- */

static const hv_reg_t REGS[31] = {
    HV_REG_X0, HV_REG_X1, HV_REG_X2, HV_REG_X3, HV_REG_X4, HV_REG_X5, HV_REG_X6, HV_REG_X7, HV_REG_X8, HV_REG_X9, HV_REG_X10,
    HV_REG_X11, HV_REG_X12, HV_REG_X13, HV_REG_X14, HV_REG_X15, HV_REG_X16, HV_REG_X17, HV_REG_X18, HV_REG_X19, HV_REG_X20, HV_REG_X21,
    HV_REG_X22, HV_REG_X23, HV_REG_X24, HV_REG_X25, HV_REG_X26, HV_REG_X27, HV_REG_X28, HV_REG_X29, HV_REG_X30};

uint64_t vm_x(unsigned n)
{
    uint64_t v = 0;
    if (n == 31) hv_vcpu_get_sys_reg(V.cpu, HV_SYS_REG_SP_EL1, &v);
    else if (n < 31) hv_vcpu_get_reg(V.cpu, REGS[n], &v);
    return v;
}

void vm_set_x(unsigned n, uint64_t v)
{
    if (n == 31) hv_vcpu_set_sys_reg(V.cpu, HV_SYS_REG_SP_EL1, v);
    else if (n < 31) hv_vcpu_set_reg(V.cpu, REGS[n], v);
}

uint64_t vm_pc(void)
{
    uint64_t v = 0;
    hv_vcpu_get_reg(V.cpu, HV_REG_PC, &v);
    return v;
}

void vm_set_pc(uint64_t pc)
{
    hv_vcpu_set_reg(V.cpu, HV_REG_PC, pc);
    V.redirected = 1;
}

void vm_context_save(struct vm_context *c)
{
    unsigned i;
    for (i = 0; i < 31; i++) c->x[i] = vm_x(i);
    c->sp = vm_x(31);
    c->pc = vm_pc();
    hv_vcpu_get_reg(V.cpu, HV_REG_CPSR, &c->cpsr);
}

void vm_context_load(const struct vm_context *c)
{
    unsigned i;
    for (i = 0; i < 31; i++) vm_set_x(i, c->x[i]);
    vm_set_x(31, c->sp);
    hv_vcpu_set_reg(V.cpu, HV_REG_PC, c->pc);
    hv_vcpu_set_reg(V.cpu, HV_REG_CPSR, c->cpsr);
    V.redirected = 1;
}

/* ---- the hooks ---- */

int vm_hostcall_set(unsigned number, const char *name, vm_hostcall_fn fn)
{
    if (number == 0 || number >= VM_HOSTCALL_MAX) return -1;
    V.calls[number].name = name;
    V.calls[number].fn = fn;
    return 0;
}

const char *vm_hostcall_name(unsigned number)
{
    return number < VM_HOSTCALL_MAX ? V.calls[number].name : NULL;
}

void vm_mmio_set(vm_mmio_read_fn read, vm_mmio_write_fn write) { V.mmio_read = read; V.mmio_write = write; }
void vm_fault_set(vm_fault_fn fn) { V.fault_fn = fn; }
void vm_interrupt_set(vm_interrupt_fn fn) { V.interrupt_fn = fn; }

void vm_interrupt(void)
{
    hv_vcpu_t cpu = V.cpu;
    hv_vcpus_exit(&cpu, 1);
}

/* ---- running ---- */

static void fault(unsigned long long ipa, const char *fmt, ...)
{
    struct vm_fault *f = &V.fault;
    va_list ap;
    unsigned i;
    uint64_t v = 0;

    memset(f, 0, sizeof *f);
    va_start(ap, fmt);
    vsnprintf(f->what, sizeof f->what, fmt, ap);
    va_end(ap);
    for (i = 0; i < 31; i++) f->x[i] = vm_x(i);
    f->sp = vm_x(31);
    f->pc = vm_pc();
    hv_vcpu_get_sys_reg(V.cpu, HV_SYS_REG_ESR_EL1, &v); f->esr = v;
    hv_vcpu_get_sys_reg(V.cpu, HV_SYS_REG_FAR_EL1, &v); f->far = v;
    hv_vcpu_get_sys_reg(V.cpu, HV_SYS_REG_ELR_EL1, &v); f->elr = v;
    hv_vcpu_get_sys_reg(V.cpu, HV_SYS_REG_SPSR_EL1, &v); f->spsr = v;
    f->ipa = ipa;
    if (V.fault_fn) V.fault_fn(f);
}

static const char *const VECTOR_NAME[16] = {
    "sync, current level, own stack 0", "irq, current level, stack 0", "fiq, current level, stack 0", "serror, current level, stack 0",
    "sync, current level", "irq, current level", "fiq, current level", "serror, current level",
    "sync, lower level", "irq, lower level", "fiq, lower level", "serror, lower level",
    "sync, lower level 32-bit", "irq, lower level 32-bit", "fiq, lower level 32-bit", "serror, lower level 32-bit"};

static const char *class_name(unsigned ec)
{
    switch (ec) {
    case 0x00: return "unknown instruction";
    case 0x20: case 0x21: return "instruction abort";
    case 0x22: return "pc not aligned";
    case 0x24: case 0x25: return "data abort";
    case 0x26: return "sp not aligned";
    case 0x3c: return "brk";
    default: return "exception";
    }
}

/* Serve one exit. 1: go on; 0: the call into the guest has returned; -1: a fault. */
static int serve(void)
{
    uint64_t syn = V.exit->exception.syndrome;
    unsigned ec = (unsigned)(syn >> 26), imm = (unsigned)(syn & 0xffff);

    if (ec == 0x16) {                                    /* hvc */
        if (imm == 0) return 0;
        if (imm >= VM_HOSTCALL_MAX) {
            uint64_t esr = 0;
            hv_vcpu_get_sys_reg(V.cpu, HV_SYS_REG_ESR_EL1, &esr);
            fault(0, "guest exception, %s: %s (class 0x%02x)", VECTOR_NAME[imm & 15], class_name((unsigned)(esr >> 26)), (unsigned)(esr >> 26));
            return -1;
        }
        if (!V.calls[imm].fn) {
            fault(0, "hvc #%u has no routine", imm);
            return -1;
        }
        {
            uint64_t a[8], lr = vm_x(30), r;
            unsigned i;
            for (i = 0; i < 8; i++) a[i] = vm_x(i) & 0xffffffffull;
            V.redirected = 0;
            r = V.calls[imm].fn(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
            if (!V.redirected) {
                vm_set_x(0, r);
                hv_vcpu_set_reg(V.cpu, HV_REG_PC, lr);
            }
        }
        return 1;
    }
    if (ec == 0x24) {                                    /* data abort at stage 2: the I/O page */
        uint64_t ipa = V.exit->exception.physical_address;
        unsigned iss = (unsigned)(syn & 0x1ffffff);
        int isv = (iss >> 24) & 1, sse = (iss >> 21) & 1, srt = (iss >> 16) & 31, sf = (iss >> 15) & 1, wnr = (iss >> 6) & 1;
        unsigned size = 1u << ((iss >> 22) & 3);
        uint64_t pc = vm_pc(), writeback = 0;
        int wb_reg = -1;
        if (ipa < VM_IO_BASE || ipa >= VM_IO_BASE + VM_IO_SIZE) {
            fault(ipa, "%s of unmapped memory at 0x%llx (address 0x%llx)", wnr ? "write" : "read", (unsigned long long)ipa,
                  (unsigned long long)V.exit->exception.virtual_address);
            return -1;
        }
        if (!isv) {
            /* A load or store with a write back of its base register: the exit does not describe it, the instruction does. */
            const uint8_t *p = vm_g((uint32_t)pc, 4);
            uint32_t insn = p ? (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24 : 0;
            unsigned idx = (insn >> 10) & 3, opc = (insn >> 22) & 3;
            if (p && (insn & 0x3b200000u) == 0x38000000u && (idx == 1 || idx == 3) && (insn & 0x04000000u) == 0) {
                int32_t imm = (int32_t)((insn >> 12) & 0x1ff);
                unsigned rn = (insn >> 5) & 31;
                if (imm & 0x100) imm -= 0x200;
                size = 1u << (insn >> 30);
                srt = (int)(insn & 31);
                wnr = opc == 0;
                sse = opc >= 2;
                sf = opc == 2 || (opc == 1 && size == 8);
                wb_reg = (int)rn;
                writeback = vm_x(rn) + (uint64_t)(int64_t)imm;
                if (opc == 3 && size == 8) wb_reg = -2;      /* no such form */
            } else wb_reg = -2;
            if (wb_reg == -2) {
                fault(ipa, "access to the I/O page at 0x%llx by the instruction 0x%08x, a form that is not served", (unsigned long long)ipa, insn);
                return -1;
            }
        }
        if (wnr) {
            uint64_t v = srt == 31 ? 0 : vm_x((unsigned)srt);
            if (size < 8) v &= (1ull << (size * 8)) - 1;
            if (V.mmio_write) V.mmio_write((uint32_t)ipa, size, v);
        } else {
            uint64_t v = V.mmio_read ? V.mmio_read((uint32_t)ipa, size) : 0;
            if (size < 8) {
                v &= (1ull << (size * 8)) - 1;
                if (sse && (v >> (size * 8 - 1))) v |= ~((1ull << (size * 8)) - 1);
            }
            if (!sf) v &= 0xffffffffull;
            if (srt != 31) vm_set_x((unsigned)srt, v);
        }
        if (wb_reg >= 0) vm_set_x((unsigned)wb_reg, writeback);
        hv_vcpu_set_reg(V.cpu, HV_REG_PC, pc + 4);
        return 1;
    }
    fault(V.exit->exception.physical_address, "exit of class 0x%02x (%s), syndrome 0x%llx, address 0x%llx", ec, class_name(ec), (unsigned long long)syn,
          (unsigned long long)V.exit->exception.virtual_address);
    return -1;
}

static int run(void)
{
    for (;;) {
        int s;
        if (hv_vcpu_run(V.cpu) != HV_SUCCESS) {
            fault(0, "hv_vcpu_run failed");
            return -1;
        }
        V.started = 1;
        switch (V.exit->reason) {
        case HV_EXIT_REASON_CANCELED:
            if (V.interrupt_fn) V.interrupt_fn();
            continue;
        case HV_EXIT_REASON_VTIMER_ACTIVATED:
            hv_vcpu_set_vtimer_mask(V.cpu, false);
            continue;
        case HV_EXIT_REASON_EXCEPTION:
            s = serve();
            if (s == 1) continue;
            return s == 0 ? 0 : -1;
        default:
            fault(0, "exit of reason %d", (int)V.exit->reason);
            return -1;
        }
    }
}

int vm_call(uint32_t address, const uint64_t *args, unsigned nargs, uint64_t *result)
{
    struct vm_context saved;
    int saved_redirect = V.redirected, rc;
    unsigned i;
    uint64_t r;

    vm_context_save(&saved);
    for (i = 0; i < 8; i++) vm_set_x(i, i < nargs ? args[i] : 0);
    vm_set_x(30, TRAMP_RET);
    vm_set_x(31, (saved.sp - 256) & ~15ull);
    hv_vcpu_set_reg(V.cpu, HV_REG_PC, address);
    hv_vcpu_set_reg(V.cpu, HV_REG_CPSR, CPSR_EL1H);
    rc = run();
    r = vm_x(0);
    vm_context_load(&saved);
    V.redirected = saved_redirect;
    if (result) *result = r;
    return rc;
}

void vm_sync(void)
{
    if (V.started) vm_call(TRAMP_SYNC, NULL, 0, NULL);
}

/* ---- the machine ---- */

static void put32(uint32_t ipa, uint32_t word)
{
    memcpy(priv_at(ipa), &word, 4);
}

static int fail(char *err, size_t errsize, const char *what, long code)
{
    snprintf(err, errsize, "vm: %s (%ld)", what, code);
    return -1;
}

int vm_create(char *err, size_t errsize)
{
    uint32_t i;
    hv_return_t r;
    uint64_t sctlr = 0;

    if (V.created) return fail(err, errsize, "the machine already exists", 0);
    if ((r = hv_vm_create(NULL)) != HV_SUCCESS)
        return fail(err, errsize, "hv_vm_create failed; the program needs the com.apple.security.hypervisor entitlement (sign it, see README.md)", r);
    V.ram = mmap(NULL, VM_RAM_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    V.scratch = mmap(NULL, SCRATCH_ALLOC, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    V.priv = mmap(NULL, PRIV_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (V.ram == MAP_FAILED || V.scratch == MAP_FAILED || V.priv == MAP_FAILED) return fail(err, errsize, "cannot allocate the guest's memory", 0);
    if ((r = hv_vm_map(V.ram, VM_RAM_BASE, VM_RAM_SIZE, HV_MEMORY_READ | HV_MEMORY_WRITE | HV_MEMORY_EXEC)) != HV_SUCCESS)
        return fail(err, errsize, "cannot map the RAM", r);
    if ((r = hv_vm_map(V.scratch, SCRATCH_IPA, SCRATCH_ALLOC, HV_MEMORY_READ | HV_MEMORY_WRITE)) != HV_SUCCESS)
        return fail(err, errsize, "cannot map the scratchpad", r);
    if ((r = hv_vm_map(V.priv, VM_IMAGE_BASE, PRIV_SIZE, HV_MEMORY_READ | HV_MEMORY_WRITE | HV_MEMORY_EXEC)) != HV_SUCCESS)
        return fail(err, errsize, "cannot map the private area", r);
    V.brk = VM_IMAGE_LIMIT;
    V.pool_next = POOL_IPA;

    /* vectors: every entry is `hvc #(0xff00 + index)`; the routines that the host calls */
    for (i = 0; i < 16; i++) put32(VECTORS + 0x80 * i, 0xd4000002u | ((0xff00u + i) << 5));
    put32(TRAMP_RET, 0xd4000002u);
    put32(TRAMP_SYNC + 0, 0xd508751fu);   /* ic iallu */
    put32(TRAMP_SYNC + 4, 0xd5033b9fu);   /* dsb ish */
    put32(TRAMP_SYNC + 8, 0xd508871fu);   /* tlbi vmalle1 */
    put32(TRAMP_SYNC + 12, 0xd5033b9fu);  /* dsb ish */
    put32(TRAMP_SYNC + 16, 0xd5033fdfu);  /* isb */
    put32(TRAMP_SYNC + 20, 0xd65f03c0u);  /* ret */

    /* the guest's view */
    if (!(V.l1 = (uint64_t *)priv_at(V.pool_next))) return fail(err, errsize, "no page table", 0);
    memset(V.l1, 0, 0x1000);
    V.pool_next += 0x1000;
    if (map(0x00000000, VM_RAM_BASE, VM_RAM_SIZE, VM_R | VM_W | VM_X, ATTR_NORMAL) ||
        map(VM_RAM_BASE, VM_RAM_BASE, VM_RAM_SIZE, VM_R | VM_W | VM_X, ATTR_NORMAL) ||
        map(0xa0000000, VM_RAM_BASE, VM_RAM_SIZE, VM_R | VM_W | VM_X, ATTR_NORMAL) ||
        map(VM_SCRATCH_BASE, SCRATCH_IPA, VM_SCRATCH_PAGE, VM_R | VM_W, ATTR_NORMAL) ||
        map(VM_IO_BASE, VM_IO_BASE, VM_IO_SIZE, VM_R | VM_W, ATTR_DEVICE) ||
        map(VM_IMAGE_BASE, VM_IMAGE_BASE, VM_PRIVATE_LIMIT - VM_IMAGE_BASE, VM_R | VM_W, ATTR_NORMAL) ||
        map(VECTORS, VECTORS, TRAMP_END - VECTORS, VM_R | VM_X, ATTR_NORMAL))
        return fail(err, errsize, "the page tables do not fit", 0);

    if ((r = hv_vcpu_create(&V.cpu, &V.exit, NULL)) != HV_SUCCESS) return fail(err, errsize, "hv_vcpu_create failed", r);
    hv_vcpu_set_sys_reg(V.cpu, HV_SYS_REG_MAIR_EL1, 0x00ffull);              /* attribute 0: normal, write back; 1: device */
    hv_vcpu_set_sys_reg(V.cpu, HV_SYS_REG_TCR_EL1, 0x20ull | (1ull << 8) | (1ull << 10) | (3ull << 12) | (1ull << 23) | (2ull << 30) | (1ull << 32));
    hv_vcpu_set_sys_reg(V.cpu, HV_SYS_REG_TTBR0_EL1, POOL_IPA);
    hv_vcpu_set_sys_reg(V.cpu, HV_SYS_REG_VBAR_EL1, VECTORS);
    hv_vcpu_set_sys_reg(V.cpu, HV_SYS_REG_CPACR_EL1, 3ull << 20);
    hv_vcpu_get_sys_reg(V.cpu, HV_SYS_REG_SCTLR_EL1, &sctlr);
    sctlr |= (1ull << 0) | (1ull << 2) | (1ull << 12);                        /* MMU, data cache, instruction cache */
    sctlr &= ~((1ull << 1) | (1ull << 3) | (1ull << 4) | (1ull << 19));       /* no alignment check, stack checks, write-implies-no-execute */
    hv_vcpu_set_sys_reg(V.cpu, HV_SYS_REG_SCTLR_EL1, sctlr);
    hv_vcpu_set_reg(V.cpu, HV_REG_CPSR, CPSR_EL1H);
    V.created = 1;
    return 0;
}

void vm_destroy(void)
{
    if (!V.created) return;
    hv_vcpu_destroy(V.cpu);
    hv_vm_destroy();
    munmap(V.ram, VM_RAM_SIZE);
    munmap(V.scratch, SCRATCH_ALLOC);
    munmap(V.priv, PRIV_SIZE);
    memset(&V, 0, sizeof V);
}
