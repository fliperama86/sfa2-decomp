/* The guest side of the machine's controls (test_vm.py): small functions that the host calls and checks. Built with
 * `aarch64-elf-gcc -mabi=ilp32 -O1 -ffreestanding -nostdlib` and linked with the guest linker script and the host stubs. */
struct node {
    struct node *next;
    int v;
};

extern unsigned port_test_add(unsigned a, unsigned b);
extern unsigned port_test_callback(unsigned function, unsigned argument);
extern unsigned port_test_ctx(unsigned which);

volatile unsigned g_flag;
unsigned g_data = 7;

int g_sum(struct node *n)
{
    int t = 0;
    while (n) {
        t += n->v;
        n = n->next;
    }
    return t;
}

/* A word written through one view of the RAM is read through the other two. */
unsigned g_alias(void)
{
    volatile unsigned *a = (volatile unsigned *)0x80001000u, *b = (volatile unsigned *)0x00001000u, *c = (volatile unsigned *)0xa0001000u;
    *a = 0x12345678u;
    return (*b == 0x12345678u) + 2 * (*c == 0x12345678u);
}

unsigned g_scratch(void)
{
    volatile unsigned *s = (volatile unsigned *)0x1f800000u;
    *s = 0xcafebabeu;
    return *s;
}

/* Accesses of the I/O page: a word read and written, a half word written, a byte read. */
unsigned g_mmio(void)
{
    volatile unsigned *p = (volatile unsigned *)0x1f801070u;
    unsigned v = *p;
    *p = v + 1;
    *(volatile unsigned short *)0x1f801074u = 0xabcdu;
    return v + *(volatile unsigned char *)0x1f801078u;
}

unsigned g_hostcall(void)
{
    return port_test_add(40, 2);
}

unsigned g_nested_callee(unsigned x)
{
    return port_test_add(x, 1000);
}

/* The host calls g_nested_callee back, which calls the host again. */
unsigned g_nested(void)
{
    return port_test_callback((unsigned)g_nested_callee, 5) + 1;
}

unsigned g_fib(unsigned n)
{
    return n < 2 ? n : g_fib(n - 1) + g_fib(n - 2);
}

unsigned g_read_unmapped(void)
{
    return *(volatile unsigned *)0x40000000u;
}

unsigned g_undefined(void)
{
    __asm__ volatile(".word 0x00000000");
    return 0;
}

void g_write_code(void)
{
    *(volatile unsigned *)(unsigned)g_sum = 0;
}

unsigned g_run_data(void)
{
    ((void (*)(void))&g_data)();
    return 1;
}

void g_spin(void)
{
    while (!g_flag) {
    }
}

/* Two tasks that hand control to each other through the host: the sum of what each sees. */
unsigned g_task_a(void)
{
    unsigned t = 0, i;
    for (i = 0; i < 3; i++) t += port_test_ctx(1);
    return t;
}

unsigned g_task_b(void)
{
    for (;;) port_test_ctx(2);
}

/* The forms with a write back of the base register, which the exit does not describe: a post-indexed word load, a pre-indexed word
 * store and a post-indexed signed byte load. Returns the base registers' steps and the byte. */
unsigned g_mmio_forms(void)
{
    unsigned long p = 0x1f801090ul, q = 0x1f801100ul;
    unsigned v;
    int sb;
    __asm__ volatile("ldr %w0, [%1], #8" : "=r"(v), "+r"(p) : : "memory");
    __asm__ volatile("str %w1, [%0, #4]!" : "+r"(q) : "r"(v) : "memory");
    __asm__ volatile("ldrsb %w0, [%1], #1" : "=r"(sb), "+r"(p) : : "memory");
    return ((unsigned)(p - 0x1f801090ul) << 24) | ((unsigned)(q - 0x1f801100ul) << 16) | ((unsigned)sb & 0xffffu);
}

/* A page of the RAM that the host made not executable until it is called. */
unsigned g_run_ram(void)
{
    ((void (*)(void))0x80101000u)();
    return 5;
}

void g_returns(void)
{
}

/* The host starts a task whose function returns (to the exit trampoline) and goes back here when it has. */
unsigned g_exit_demo(void)
{
    return port_test_ctx(3);
}
