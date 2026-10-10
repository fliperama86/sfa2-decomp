/* The generic part of the native replay: runs ONE function of the game's C, as compiled for the PC, on cases
 * that arrive as text on standard input, and writes what happened as text on standard output. The formats are
 * described in README.md. The program is linked by port/tools/hostreplay.py with a made-up file that sets
 * `replay_target` and with the function's object, every name of which is at its PS1 address.
 *
 * The file has two parts. The generic part (the reading of cases, the stand-ins, the log, the comparison of memory,
 * the writing of results) comes first and knows nothing of a system. The system part is last and is the only place
 * that knows Windows and the x86 call: the mapping of memory at the PS1's addresses, the pages without execute
 * permission, the handler for the faults, the call of the function and the return from a fault. A second system
 * is added there and nowhere else.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAM_BASE 0x80000000u
#define RAM_SIZE 0x200000u
#define SCRATCH_BASE 0x1F800000u
#define SCRATCH_SIZE 0x1000u
#define PAGE 0x1000u

#define MAX_ARGUMENTS 16
#define MAX_STUBS 64
#define MAX_RETURNS 256
#define MAX_STORES 64
#define MAX_COUNTS 64
#define MAX_WATCH 32
#define MAX_CALLS 100000u   /* more calls of stand-ins than this end the case as a fault of kind "calls" */

/* How a call of the function ended. */
enum { END_RETURNED = 0, END_STUB = 1, END_FAULT = 2 };

/* The system part. */
extern void sys_start(unsigned limit_seconds);
extern int sys_map(void);
extern void sys_pages(unsigned base, unsigned size, int execute);
extern unsigned sys_stack_base(void);
extern unsigned sys_entry(void);
extern void sys_case(int running);
extern int sys_call(const unsigned *words, void *target);
extern void sys_unwind(int reason);
extern unsigned sys_result(void);
extern void *replay_target_pointer(void);

/* Set by the system part when it catches a fault, read here. */
struct fault_record {
    const char *kind;   /* exec, read, write, other */
    unsigned address;
    unsigned pc;
};
struct fault_record replay_fault;

struct stub {
    unsigned at, arguments, ends_at, calls;
    int masked[MAX_ARGUMENTS];
    unsigned mask[MAX_ARGUMENTS];
    unsigned pointee[MAX_ARGUMENTS];   /* words behind the argument; 0: none */
    unsigned nreturns, returns[MAX_RETURNS];
    unsigned nstores;
    struct { unsigned n, address, value; } stores[MAX_STORES];
    unsigned ncounts, counts[MAX_COUNTS];
};

static struct stub stubs[MAX_STUBS];
static unsigned nstubs;
static struct { unsigned address, words; } watches[MAX_WATCH];
static unsigned nwatches;
static unsigned case_args[8];
static unsigned total_calls;

static unsigned char before_ram[RAM_SIZE];
static unsigned char before_scratch[SCRATCH_SIZE];

struct buffer {
    char *data;
    size_t size, used;
};
static struct buffer calls_log;

static void fail(const char *fmt, ...)
{
    va_list ap;
    printf("error ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
    exit(2);
}

static void put(struct buffer *b, const char *fmt, ...)
{
    va_list ap;
    int n;
    for (;;) {
        va_start(ap, fmt);
        n = vsnprintf(b->data ? b->data + b->used : 0, b->size - b->used, fmt, ap);
        va_end(ap);
        if (n < 0) fail("cannot format");
        if (b->data && (size_t)n < b->size - b->used) {
            b->used += (size_t)n;
            return;
        }
        b->size = b->size ? b->size * 2 : 4096;
        while (b->size - b->used <= (size_t)n) b->size *= 2;
        b->data = realloc(b->data, b->size);
        if (!b->data) fail("out of memory");
    }
}

static void put_hex(struct buffer *b, const unsigned char *p, size_t n)
{
    static const char digits[] = "0123456789abcdef";
    size_t i;
    if (n * 2 + 1 > b->size - b->used) {
        while (n * 2 + 1 > b->size - b->used) b->size = b->size ? b->size * 2 : 4096;
        b->data = realloc(b->data, b->size);
        if (!b->data) fail("out of memory");
    }
    for (i = 0; i < n; i++) {
        b->data[b->used++] = digits[p[i] >> 4];
        b->data[b->used++] = digits[p[i] & 15];
    }
}

/* ---- memory ----------------------------------------------------------------------------------------------- */

static int in_ram(unsigned a, unsigned long long n)
{
    return a >= RAM_BASE && a - RAM_BASE < RAM_SIZE && (unsigned long long)(a - RAM_BASE) + n <= RAM_SIZE;
}

static int in_scratch(unsigned a, unsigned long long n)
{
    return a >= SCRATCH_BASE && a - SCRATCH_BASE < SCRATCH_SIZE && (unsigned long long)(a - SCRATCH_BASE) + n <= SCRATCH_SIZE;
}

static void need_memory(unsigned a, unsigned long long n, const char *what)
{
    if (!in_ram(a, n) && !in_scratch(a, n)) fail("%s: %x length %llx is not in memory", what, a, n);
}

static void fill_all(void)
{
    memset((void *)RAM_BASE, 0xA5, RAM_SIZE);
    memset((void *)SCRATCH_BASE, 0xA5, SCRATCH_SIZE);
}

/* ---- the stand-ins ---------------------------------------------------------------------------------------- */

static int is_stack_address(unsigned v, unsigned low)
{
    return v >= low && v < sys_stack_base();
}

static unsigned word_at(unsigned a)
{
    return *(volatile unsigned *)(size_t)a;
}

/* Called by the common entry of the system part with the address after the five-byte call that stands at a stub
 * and a pointer to the arguments, as the function pushed them. Returns the stand-in's result. */
unsigned replay_stub(unsigned after, const unsigned *args);
unsigned __attribute__((used)) replay_stub(unsigned after, const unsigned *args)
{
    unsigned at = after - 5, i, k, n;
    struct stub *s = 0;
    struct buffer entry = { 0, 0, 0 };
    unsigned char *tmp[MAX_ARGUMENTS] = { 0 };
    unsigned values[MAX_ARGUMENTS];
    int stack[MAX_ARGUMENTS];
    unsigned char *watched[MAX_WATCH] = { 0 };

    for (i = 0; i < nstubs; i++)
        if (stubs[i].at == at) s = &stubs[i];
    if (!s) {
        replay_fault.kind = "other";
        replay_fault.address = at;
        replay_fault.pc = at;
        sys_unwind(END_FAULT);
    }
    if (total_calls >= MAX_CALLS) {
        replay_fault.kind = "calls";
        replay_fault.address = at;
        replay_fault.pc = at;
        sys_unwind(END_FAULT);
    }
    /* Read everything first (a pointee that cannot be read is a fault of the case, and then no half entry is logged). */
    for (i = 0; i < s->arguments; i++) {
        stack[i] = is_stack_address(args[i], (unsigned)(size_t)args);
        values[i] = stack[i] ? 0 : (s->masked[i] ? args[i] & s->mask[i] : args[i]);
        if (s->pointee[i]) {
            n = s->pointee[i] * 4;
            tmp[i] = malloc(n);
            if (!tmp[i]) fail("out of memory");
            memcpy(tmp[i], (const void *)(size_t)args[i], n);
        }
    }
    for (k = 0; k < nwatches; k++) {
        watched[k] = malloc(watches[k].words * 4);
        if (!watched[k]) fail("out of memory");
        memcpy(watched[k], (const void *)(size_t)watches[k].address, watches[k].words * 4);
    }
    put(&entry, "call %x %u", at, s->arguments);
    for (i = 0; i < s->arguments; i++) put(&entry, " %x", values[i]);
    put(&entry, "\n");
    for (i = 0; i < s->arguments; i++) {
        if (stack[i]) put(&entry, "stack %u\n", i);
        if (s->pointee[i]) {
            put(&entry, "pointee %u ", i);
            put_hex(&entry, tmp[i], s->pointee[i] * 4);
            put(&entry, "\n");
        }
    }
    for (k = 0; k < nwatches; k++) {
        put(&entry, "watched ");
        put_hex(&entry, watched[k], watches[k].words * 4);
        put(&entry, "\n");
        free(watched[k]);
    }
    for (i = 0; i < s->arguments; i++) free(tmp[i]);
    put(&calls_log, "%s", entry.data);
    free(entry.data);
    total_calls++;
    s->calls++;
    for (i = 0; i < s->ncounts; i++) *(volatile unsigned *)(size_t)s->counts[i] = word_at(s->counts[i]) + 1;
    for (i = 0; i < s->nstores; i++)
        if (s->stores[i].n == s->calls) *(volatile unsigned *)(size_t)s->stores[i].address = s->stores[i].value;
    if (s->ends_at && s->calls == s->ends_at) sys_unwind(END_STUB);
    if (s->nreturns == 0) return 0;
    return s->returns[s->calls <= s->nreturns ? s->calls - 1 : s->nreturns - 1];
}

/* ---- reading cases ---------------------------------------------------------------------------------------- */

static char *next_token(char **p)
{
    char *s = *p, *t;
    while (*s == ' ' || *s == '\t' || *s == '\r') s++;
    if (!*s) {
        *p = s;
        return 0;
    }
    t = s;
    while (*s && *s != ' ' && *s != '\t' && *s != '\r') s++;
    if (*s) *s++ = 0;
    *p = s;
    return t;
}

static unsigned number(const char *s, const char *line)
{
    char *end;
    unsigned long long v;
    if (!s || !*s) fail("a number is missing in: %s", line);
    v = strtoull(s, &end, 16);
    if (*end || v > 0xFFFFFFFFull) fail("bad number '%s' in: %s", s, line);
    return (unsigned)v;
}

static int hex_digit(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static struct stub *last_stub(const char *line)
{
    if (!nstubs) fail("no stub is open for: %s", line);
    return &stubs[nstubs - 1];
}

static void start_case(void)
{
    nstubs = 0;
    nwatches = 0;
    total_calls = 0;
    memset(case_args, 0, sizeof case_args);
    free(calls_log.data);
    calls_log.data = 0;
    calls_log.size = calls_log.used = 0;
    sys_pages(RAM_BASE, RAM_SIZE, 0);
    sys_pages(SCRATCH_BASE, SCRATCH_SIZE, 0);
    fill_all();
}

/* One line of a case; returns 1 at `go`. `work` is a copy of `line` that the reading may cut into tokens. */
static int case_line_work(char *work, const char *line)
{
    char *p = work, *cmd = next_token(&p), *a;
    struct stub *s;
    unsigned x, y, z, i;

    if (!cmd) return 0;
    if (!strcmp(cmd, "fill")) {
        x = number(next_token(&p), line);
        y = number(next_token(&p), line);
        z = number(next_token(&p), line);
        if (z > 255) fail("a byte above ff in: %s", line);
        need_memory(x, y, "fill");
        memset((void *)(size_t)x, (int)z, y);
    } else if (!strcmp(cmd, "bytes")) {
        x = number(next_token(&p), line);
        a = next_token(&p);
        y = a ? (unsigned)strlen(a) : 0;
        if (y % 2) fail("odd count of hex digits in: %s", line);
        need_memory(x, y / 2, "bytes");
        for (i = 0; i < y / 2; i++) {
            int hi = hex_digit(a[2 * i]), lo = hex_digit(a[2 * i + 1]);
            if (hi < 0 || lo < 0) fail("bad hex digit in: %s", line);
            ((unsigned char *)(size_t)x)[i] = (unsigned char)(hi << 4 | lo);
        }
    } else if (!strcmp(cmd, "args")) {
        for (i = 0; (a = next_token(&p)) != 0; i++) {
            if (i >= 8) fail("more than eight arguments in: %s", line);
            case_args[i] = number(a, line);
        }
    } else if (!strcmp(cmd, "stub")) {
        if (nstubs >= MAX_STUBS) fail("more than %d stubs", MAX_STUBS);
        s = &stubs[nstubs];
        memset(s, 0, sizeof *s);
        s->at = number(next_token(&p), line);
        s->arguments = number(next_token(&p), line);
        s->ends_at = number(next_token(&p), line);
        if (s->arguments > MAX_ARGUMENTS) fail("more than %d arguments in: %s", MAX_ARGUMENTS, line);
        if (!in_ram(s->at, 5)) fail("a stub must lie in RAM: %s", line);
        for (i = 0; i < nstubs; i++)
            if (stubs[i].at < s->at + 5 && s->at < stubs[i].at + 5) fail("two stubs overlap: %s", line);
        nstubs++;
    } else if (!strcmp(cmd, "mask")) {
        s = last_stub(line);
        x = number(next_token(&p), line);
        y = number(next_token(&p), line);
        if (x >= s->arguments) fail("mask for an argument the stub does not take: %s", line);
        s->masked[x] = 1;
        s->mask[x] = y;
    } else if (!strcmp(cmd, "pointee")) {
        s = last_stub(line);
        x = number(next_token(&p), line);
        y = number(next_token(&p), line);
        if (x >= s->arguments || y < 1 || y > 0x40000) fail("bad pointee: %s", line);
        s->pointee[x] = y;
    } else if (!strcmp(cmd, "return")) {
        s = last_stub(line);
        if (s->nreturns >= MAX_RETURNS) fail("too many returns: %s", line);
        s->returns[s->nreturns++] = number(next_token(&p), line);
    } else if (!strcmp(cmd, "store")) {
        s = last_stub(line);
        if (s->nstores >= MAX_STORES) fail("too many stores: %s", line);
        x = number(next_token(&p), line);
        y = number(next_token(&p), line);
        z = number(next_token(&p), line);
        if (x < 1 || y % 4) fail("a store needs a call number from 1 and an aligned address: %s", line);
        need_memory(y, 4, "store");
        s->stores[s->nstores].n = x;
        s->stores[s->nstores].address = y;
        s->stores[s->nstores].value = z;
        s->nstores++;
    } else if (!strcmp(cmd, "count")) {
        s = last_stub(line);
        if (s->ncounts >= MAX_COUNTS) fail("too many counts: %s", line);
        x = number(next_token(&p), line);
        if (x % 4) fail("a counted word must be aligned: %s", line);
        need_memory(x, 4, "count");
        s->counts[s->ncounts++] = x;
    } else if (!strcmp(cmd, "watch")) {
        if (nwatches >= MAX_WATCH) fail("too many watched blocks");
        x = number(next_token(&p), line);
        y = number(next_token(&p), line);
        if (y < 1 || y > 0x40000) fail("bad watch: %s", line);
        need_memory(x, (unsigned long long)y * 4, "watch");
        watches[nwatches].address = x;
        watches[nwatches].words = y;
        nwatches++;
    } else if (!strcmp(cmd, "go")) {
        return 1;
    } else {
        fail("unknown line: %s", line);
    }
    return 0;
}

static int case_line(const char *line)
{
    char *work = strdup(line);
    int r;
    if (!work) fail("out of memory");
    r = case_line_work(work, line);
    free(work);
    return r;
}

/* ---- running a case --------------------------------------------------------------------------------------- */

static int is_stub_byte(unsigned a)
{
    unsigned i;
    for (i = 0; i < nstubs; i++)
        if (a >= stubs[i].at && a < stubs[i].at + 5) return 1;
    return 0;
}

static void changes(struct buffer *out, unsigned base, const unsigned char *before, unsigned size)
{
    const unsigned char *now = (const unsigned char *)(size_t)base;
    unsigned i = 0, j;
    while (i < size) {
        if (now[i] == before[i] || is_stub_byte(base + i)) {
            i++;
            continue;
        }
        for (j = i; j < size && now[j] != before[j] && !is_stub_byte(base + j); j++) {
        }
        put(out, "change %x %u ", base + i, j - i);
        put_hex(out, now + i, j - i);
        put(out, "\n");
        i = j;
    }
}

static void run_case(void)
{
    unsigned i, p;
    int end;
    struct buffer out = { 0, 0, 0 };

    memcpy(before_ram, (void *)RAM_BASE, RAM_SIZE);
    memcpy(before_scratch, (void *)SCRATCH_BASE, SCRATCH_SIZE);
    for (i = 0; i < nstubs; i++) {
        unsigned char *at = (unsigned char *)(size_t)stubs[i].at;
        unsigned rel = sys_entry() - (stubs[i].at + 5);
        for (p = stubs[i].at & ~(PAGE - 1); p < stubs[i].at + 5; p += PAGE) sys_pages(p, PAGE, 1);
        at[0] = 0xE8;
        at[1] = (unsigned char)rel;
        at[2] = (unsigned char)(rel >> 8);
        at[3] = (unsigned char)(rel >> 16);
        at[4] = (unsigned char)(rel >> 24);
    }
    replay_fault.kind = "other";
    replay_fault.address = replay_fault.pc = 0;
    sys_case(1);
    end = sys_call(case_args, replay_target_pointer());
    sys_case(0);
    sys_pages(RAM_BASE, RAM_SIZE, 0);
    sys_pages(SCRATCH_BASE, SCRATCH_SIZE, 0);

    put(&out, "begin\n");
    put(&out, "end %s\n", end == END_RETURNED ? "returned" : end == END_STUB ? "stub" : "fault");
    if (end == END_RETURNED) put(&out, "result %x\n", sys_result());
    if (end == END_FAULT) put(&out, "fault %s %x %x\n", replay_fault.kind, replay_fault.address, replay_fault.pc);
    if (calls_log.data) put(&out, "%s", calls_log.data);
    changes(&out, RAM_BASE, before_ram, RAM_SIZE);
    changes(&out, SCRATCH_BASE, before_scratch, SCRATCH_SIZE);
    put(&out, "done\n");
    fwrite(out.data, 1, out.used, stdout);
    fflush(stdout);
    free(out.data);
}

int main(int argc, char **argv)
{
    unsigned limit = 0;
    char *input = 0, *line;
    size_t used = 0, size = 0, n;

    if (argc == 3 && !strcmp(argv[1], "--limit")) limit = (unsigned)strtoul(argv[2], 0, 10);
    else if (argc != 1) fail("usage: PROGRAM [--limit SECONDS]");
    sys_start(limit);
    if (!sys_map()) fail("cannot map the PS1's memory");
    for (;;) {
        if (used + 65536 > size) {
            size = size ? size * 2 : 65536;
            input = realloc(input, size);
            if (!input) fail("out of memory");
        }
        n = fread(input + used, 1, size - used - 1, stdin);
        if (n == 0) break;
        used += n;
    }
    if (!input) return 0;
    input[used] = 0;
    for (line = input; *line;) {
        char *end = strchr(line, '\n'), *work;
        size_t len;
        if (end) *end = 0;
        len = strlen(line);
        work = line;
        while (*work == ' ' || *work == '\t') work++;
        if (!strcmp(work, "case") || !strncmp(work, "case ", 5)) {
            start_case();
        } else if (case_line(line)) {
            run_case();
        }
        line += len + (end ? 1 : 0);
    }
    return 0;
}

/* ---- the system part: Windows on x86 ---------------------------------------------------------------------- */

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <windows.h>

#define STR2(x) #x
#define STR(x) STR2(x)
#define L(name) STR(__USER_LABEL_PREFIX__) #name

extern void *replay_target;   /* set by the made-up file the driver links */
unsigned replay_saved_esp, replay_result;
extern void replay_entry(void);

/* sys_call(words, target): pushes the eight words, calls target (cdecl), and returns END_RETURNED with the result
 * kept in replay_result. sys_unwind(reason) returns from sys_call with that reason, from anywhere below it: it
 * restores the stack and the registers a C function preserves, which sys_call saved. */
__asm__(
    ".text\n"
    ".globl " L(sys_call) "\n"
    L(sys_call) ":\n"
    "    pushl %ebp\n"
    "    pushl %ebx\n"
    "    pushl %esi\n"
    "    pushl %edi\n"
    "    movl %esp, " L(replay_saved_esp) "\n"
    "    movl 20(%esp), %edx\n"
    "    movl 24(%esp), %ecx\n"
    "    pushl 28(%edx)\n"
    "    pushl 24(%edx)\n"
    "    pushl 20(%edx)\n"
    "    pushl 16(%edx)\n"
    "    pushl 12(%edx)\n"
    "    pushl 8(%edx)\n"
    "    pushl 4(%edx)\n"
    "    pushl 0(%edx)\n"
    "    cld\n"
    "    call *%ecx\n"
    "    movl %eax, " L(replay_result) "\n"
    "    xorl %eax, %eax\n"
    "    jmp " L(replay_leave) "\n"
    ".globl " L(sys_unwind) "\n"
    L(sys_unwind) ":\n"
    "    movl 4(%esp), %eax\n"
    L(replay_leave) ":\n"
    "    movl " L(replay_saved_esp) ", %esp\n"
    "    popl %edi\n"
    "    popl %esi\n"
    "    popl %ebx\n"
    "    popl %ebp\n"
    "    ret\n"
    /* The common entry of the stubs. On entry the top of the stack is the address after the call written at the
     * stub, below it the function's own return address, then its arguments. */
    ".globl " L(replay_entry) "\n"
    L(replay_entry) ":\n"
    "    popl %eax\n"
    "    leal 4(%esp), %ecx\n"
    "    pushl %ecx\n"
    "    pushl %eax\n"
    "    call " L(replay_stub) "\n"
    "    addl $8, %esp\n"
    "    ret\n"
);

static volatile int running;
static volatile int stack_overflow;
static volatile DWORD started;
static unsigned limit_ms;

unsigned sys_result(void)
{
    return replay_result;
}

void *replay_target_pointer(void)
{
    return replay_target;
}

unsigned sys_entry(void)
{
    return (unsigned)(size_t)replay_entry;
}

unsigned sys_stack_base(void)
{
    return (unsigned)(size_t)((NT_TIB *)NtCurrentTeb())->StackBase;
}

void sys_case(int on)
{
    running = on;
    started = GetTickCount();
}

static void landing(void)
{
    if (stack_overflow) {
        stack_overflow = 0;
        _resetstkoflw();
    }
    sys_unwind(END_FAULT);
}

static LONG CALLBACK on_exception(EXCEPTION_POINTERS *p)
{
    const EXCEPTION_RECORD *r = p->ExceptionRecord;
    if (!running) return EXCEPTION_CONTINUE_SEARCH;
    if (r->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && r->NumberParameters >= 2) {
        replay_fault.kind = r->ExceptionInformation[0] == 8 ? "exec" : r->ExceptionInformation[0] == 1 ? "write" : "read";
        replay_fault.address = (unsigned)r->ExceptionInformation[1];
    } else if ((r->ExceptionCode & 0xC0000000u) == 0xC0000000u) {
        replay_fault.kind = "other";
        replay_fault.address = (unsigned)r->ExceptionCode;
        if (r->ExceptionCode == EXCEPTION_STACK_OVERFLOW) stack_overflow = 1;
    } else {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    replay_fault.pc = (unsigned)p->ContextRecord->Eip;
    running = 0;
    p->ContextRecord->Eip = (DWORD)(size_t)landing;
    p->ContextRecord->Esp = (DWORD)(replay_saved_esp - 64);
    p->ContextRecord->EFlags &= ~0x400u;
    return EXCEPTION_CONTINUE_EXECUTION;
}

/* A last resort against a program left running: it ends itself when a case has run for LIMIT seconds. The
 * driver stops a program that does not answer long before this. */
static DWORD WINAPI watchdog(LPVOID unused)
{
    (void)unused;
    for (;;) {
        Sleep(100);
        if (running && GetTickCount() - started > limit_ms) ExitProcess(3);
    }
    return 0;
}

void sys_start(unsigned limit_seconds)
{
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
    if (!AddVectoredExceptionHandler(1, on_exception)) fail("no handler for faults");
    if (limit_seconds) {
        limit_ms = limit_seconds * 1000u;
        if (!CreateThread(0, 0, watchdog, 0, 0, 0)) fail("no watchdog thread");
    }
}

int sys_map(void)
{
    return VirtualAlloc((void *)(size_t)RAM_BASE, RAM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) != 0 &&
           VirtualAlloc((void *)(size_t)SCRATCH_BASE, SCRATCH_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) != 0;
}

void sys_pages(unsigned base, unsigned size, int execute)
{
    DWORD old;
    if (!VirtualProtect((void *)(size_t)base, size, execute ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE, &old))
        fail("cannot set the permission of the pages at %x", base);
}

#else
#error "the system part of the replay harness is written for Windows only"
#endif
