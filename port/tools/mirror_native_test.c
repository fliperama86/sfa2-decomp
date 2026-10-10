/* The native part of the controls for the PS1's copy of RAM at address 0 (test_hostmirror.py builds and runs this).
 *
 * It links mirrorcore.c only and starts no part of the port's program. Each group below prints one line,
 * `ok NAME (N checks)` or `FAIL NAME: the first failure`:
 *  - the decision "is this fault served": borders at 0xffff, 0x10000, 0x1fffff and 0x200000 and the overflow of
 *    address plus size, kinds, thread, code;
 *  - the decoder: every instruction this file builds (each form, each register operand, each addressing
 *    shape) is decoded to its length, operand address and sizes; instructions that are not served are refused;
 *  - the operation: every built instruction is run on the processor (the real instruction, on real memory, with
 *    the registers and flags set) and by port_mirror_exec on an equal copy; all registers, all flags and every
 *    byte of the memory must be equal afterwards;
 *  - the start check, with invented answers of the system, over the whole range;
 *  - the image's filler section, read from invented headers (the good one and each way it can be wrong). */
#include "mirror.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

#define PORT_STR2(x) #x
#define PORT_STR(x) PORT_STR2(x)
#define US PORT_STR(__USER_LABEL_PREFIX__)

static struct group { char name[120]; unsigned n, bad; char first[240]; } groups[128];
static unsigned ngroups;

static struct group *grp(const char *name)
{
    unsigned i;
    for (i = 0; i < ngroups; i++)
        if (strcmp(groups[i].name, name) == 0) return &groups[i];
    snprintf(groups[ngroups].name, sizeof groups[ngroups].name, "%s", name);
    return &groups[ngroups++];
}
#define CHECK(G, COND, ...) do { struct group *g_ = grp(G); g_->n++; if (!(COND) && !g_->bad++) snprintf(g_->first, sizeof g_->first, __VA_ARGS__); } while (0)

/* ---- running the real instruction ---- */

unsigned tr_esp_cell, tr_ret_cell, tr_code_cell;
struct mirror_regs tr_out;
void tr_run(struct mirror_regs *s, void *code);
__asm__(".text\n.globl " US "tr_run\n" US "tr_run:\n"
        "\tpushal\n"
        "\tmovl 36(%esp), %eax\n"
        "\tmovl 40(%esp), %ecx\n"
        "\tmovl %ecx, " US "tr_code_cell\n"
        "\tmovl $tr_back, " US "tr_ret_cell\n"
        "\tmovl %esp, " US "tr_esp_cell\n"
        "\tpushl 36(%eax)\n"
        "\tpopfl\n"
        "\tmovl 4(%eax), %ecx\n\tmovl 8(%eax), %edx\n\tmovl 12(%eax), %ebx\n\tmovl 20(%eax), %ebp\n\tmovl 24(%eax), %esi\n\tmovl 28(%eax), %edi\n"
        "\tmovl 16(%eax), %esp\n"
        "\tmovl 0(%eax), %eax\n"
        "\tjmp *" US "tr_code_cell\n"
        "tr_back:\n"
        "\txchgl %esp, " US "tr_esp_cell\n"
        "\tmovl %eax, " US "tr_out\n"
        "\tmovl " US "tr_esp_cell, %eax\n"
        "\tmovl %eax, " US "tr_out+16\n"
        "\tmovl %ecx, " US "tr_out+4\n\tmovl %edx, " US "tr_out+8\n\tmovl %ebx, " US "tr_out+12\n"
        "\tmovl %ebp, " US "tr_out+20\n\tmovl %esi, " US "tr_out+24\n\tmovl %edi, " US "tr_out+28\n"
        "\tpushfl\n\tpopl " US "tr_out+36\n"
        "\tpopal\n"
        "\tret\n");

static unsigned char *code_page;

/* ---- building instructions ---- */

struct shape { int base, index, scale, disp; };   /* base/index: register number or -1; disp: 0 none, 1 disp8, 2 disp32; both -1: absolute */

/* The bytes of modrm, sib and displacement for `field` and the shape, the address `ea` being what it must come to. Returns the length. */
static unsigned build_modrm(unsigned char *p, unsigned field, const struct shape *s, unsigned ea, struct mirror_regs *regs)
{
    unsigned n = 0, mod, disp = 0, i, sc = s->scale == 1 ? 0 : s->scale == 2 ? 1 : s->scale == 4 ? 2 : 3;
    int d = s->disp;
    int idx = s->index;
    if (s->base < 0 && s->index < 0) {
        p[n++] = (unsigned char)(0x05 | field << 3);
        for (i = 0; i < 4; i++) p[n++] = (unsigned char)(ea >> (8 * i));
        return n;
    }
    if (s->base < 0) {   /* index only: disp32 */
        p[n++] = (unsigned char)(0x04 | field << 3);
        p[n++] = (unsigned char)(sc << 6 | idx << 3 | 5);
        regs->r[idx] = 3;
        disp = ea - (3u << sc);
        for (i = 0; i < 4; i++) p[n++] = (unsigned char)(disp >> (8 * i));
        return n;
    }
    if (s->base == 5 && d == 0) d = 1;   /* [ebp] needs a displacement */
    mod = d == 0 ? 0 : d == 1 ? 1 : 2;
    disp = d == 0 ? 0 : d == 1 ? 0xffffffe8u : 0xffffe000u;   /* -0x18, -0x2000 */
    if (idx >= 0) regs->r[idx] = 3;
    if (s->base == 4 || idx >= 0) {
        p[n++] = (unsigned char)(mod << 6 | field << 3 | 4);
        p[n++] = (unsigned char)(sc << 6 | (idx < 0 ? 4 : idx) << 3 | s->base);
    } else {
        p[n++] = (unsigned char)(mod << 6 | field << 3 | s->base);
    }
    regs->r[s->base] = ea - disp - (idx >= 0 ? (3u << sc) : 0);
    if (mod == 1) p[n++] = (unsigned char)disp;
    if (mod == 2) for (i = 0; i < 4; i++) p[n++] = (unsigned char)(disp >> (8 * i));
    return n;
}

struct tmpl {
    const char *group;
    int p66;
    unsigned char op[2];
    unsigned nop;
    int field;          /* -1: the register operand varies; 0..7: fixed (a group opcode's /n) */
    unsigned immb;      /* bytes of the immediate */
    unsigned msize, rsize;
    int moffs;          /* a0..a3: address in the instruction, no modrm */
    int all_shapes;
    int has_reg;        /* a register operand exists */
};

static struct tmpl tmpls[400];
static unsigned ntmpl;

static void add(const char *group, int p66, unsigned op0, int op1, int field, unsigned immb, unsigned msize, unsigned rsize, int moffs, int all, int has_reg)
{
    struct tmpl *t = &tmpls[ntmpl++];
    t->group = group;
    t->p66 = p66;
    t->op[0] = (unsigned char)op0;
    t->op[1] = (unsigned char)(op1 < 0 ? 0 : op1);
    t->nop = op1 < 0 ? 1 : 2;
    t->field = field;
    t->immb = immb;
    t->msize = msize;
    t->rsize = rsize;
    t->moffs = moffs;
    t->all_shapes = all;
    t->has_reg = has_reg;
}

static void build_templates(void)
{
    static const char *names[6] = { "add", "or", "and", "sub", "xor", "cmp" };
    static const unsigned base[6] = { 0x00, 0x08, 0x20, 0x28, 0x30, 0x38 };
    static const int field[6] = { 0, 1, 4, 5, 6, 7 };
    unsigned k, d, p;
    for (k = 0; k < 6; k++) {
        for (d = 0; d < 4; d++) {
            unsigned op = base[k] + d;
            add(names[k], 0, op, -1, -1, 0, (d & 1) ? 4 : 1, (d & 1) ? 4 : 1, 0, 0, 1);
            if (d & 1) add(names[k], 1, op, -1, -1, 0, 2, 2, 0, 0, 1);
        }
        add(names[k], 0, 0x80, -1, field[k], 1, 1, 1, 0, 0, 0);
        add(names[k], 0, 0x81, -1, field[k], 4, 4, 4, 0, 0, 0);
        add(names[k], 1, 0x81, -1, field[k], 2, 2, 2, 0, 0, 0);
        add(names[k], 0, 0x83, -1, field[k], 1, 4, 4, 0, 0, 0);
        add(names[k], 1, 0x83, -1, field[k], 1, 2, 2, 0, 0, 0);
    }
    add("test", 0, 0x84, -1, -1, 0, 1, 1, 0, 0, 1);
    add("test", 0, 0x85, -1, -1, 0, 4, 4, 0, 0, 1);
    add("test", 1, 0x85, -1, -1, 0, 2, 2, 0, 0, 1);
    add("test", 0, 0xf6, -1, 0, 1, 1, 1, 0, 0, 0);
    add("test", 0, 0xf7, -1, 0, 4, 4, 4, 0, 0, 0);
    add("test", 1, 0xf7, -1, 0, 2, 2, 2, 0, 0, 0);
    for (p = 0; p < 2; p++) {
        add("mov", 0, 0x88, -1, -1, 0, 1, 1, 0, 1, 1);
        add("mov", p, 0x89, -1, -1, 0, p ? 2 : 4, p ? 2 : 4, 0, 1, 1);
        add("mov", 0, 0x8a, -1, -1, 0, 1, 1, 0, 1, 1);
        add("mov", p, 0x8b, -1, -1, 0, p ? 2 : 4, p ? 2 : 4, 0, 1, 1);
        add("mov", 0, 0xc6, -1, 0, 1, 1, 1, 0, 0, 0);
        add("mov", p, 0xc7, -1, 0, p ? 2 : 4, p ? 2 : 4, p ? 2 : 4, 0, 0, 0);
        add("mov", 0, 0xa0, -1, -1, 0, 1, 1, 1, 0, 1);
        add("mov", p, 0xa1, -1, -1, 0, p ? 2 : 4, p ? 2 : 4, 1, 0, 1);
        add("mov", 0, 0xa2, -1, -1, 0, 1, 1, 1, 0, 1);
        add("mov", p, 0xa3, -1, -1, 0, p ? 2 : 4, p ? 2 : 4, 1, 0, 1);
        add("movzx", p, 0x0f, 0xb6, -1, 0, 1, p ? 2 : 4, 0, 1, 1);
        add("movsx", p, 0x0f, 0xbe, -1, 0, 1, p ? 2 : 4, 0, 1, 1);
    }
    add("movzx", 0, 0x0f, 0xb7, -1, 0, 2, 4, 0, 1, 1);
    add("movsx", 0, 0x0f, 0xbf, -1, 0, 2, 4, 0, 1, 1);
}

static struct shape shapes[400];
static unsigned nshape;

static void build_shapes(void)
{
    static const int idxs[4] = { -1, 1, 5, 7 };
    static const int scales[3] = { 1, 4, 8 };
    int b, i, d, s;
    shapes[nshape++] = (struct shape){ -1, -1, 1, 0 };
    for (i = 0; i < 4; i++)
        if (idxs[i] >= 0) for (s = 0; s < 3; s++) shapes[nshape++] = (struct shape){ -1, idxs[i], scales[s], 2 };
    for (b = 0; b < 8; b++)
        for (i = 0; i < 4; i++)
            for (d = 0; d < 3; d++) {
                if (idxs[i] == b) continue;
                for (s = 0; s < (idxs[i] < 0 ? 1 : 3); s++) shapes[nshape++] = (struct shape){ b, idxs[i], idxs[i] < 0 ? 1 : scales[s], d };
            }
}

static const unsigned V[14] = { 0, 1, 0x7f, 0x80, 0xff, 0x100, 0x7fff, 0x8000, 0xffff, 0x10000, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u };
static const unsigned FL[4] = { 0x202, 0x202 | 0x8d5, 0x202 | 0x0c1, 0x202 | 0x814 };

static unsigned char bufA[512], bufB[512];

static void set_reg_part(struct mirror_regs *r, unsigned num, unsigned size, unsigned v)
{
    if (size == 1) {
        if (num < 4) r->r[num] = (r->r[num] & ~0xffu) | (v & 0xff);
        else r->r[num - 4] = (r->r[num - 4] & ~0xff00u) | ((v & 0xff) << 8);
    } else if (size == 2) {
        r->r[num] = (r->r[num] & ~0xffffu) | (v & 0xffff);
    } else {
        r->r[num] = v;
    }
}

static unsigned long long runs;

static void one(const struct tmpl *t, unsigned field, const struct shape *s, unsigned vi, unsigned rv, unsigned imm, unsigned flags, unsigned misalign)
{
    unsigned char insn[32], full[48];
    unsigned n = 0, i, ea, len;
    struct mirror_regs start, real, emu;
    struct mirror_insn in;
    char gname[100];

    memset(insn, 0, sizeof insn);
    for (i = 0; i < 256; i++) bufA[i] = (unsigned char)(0x5a ^ i);
    ea = (unsigned)(size_t)bufA + 0x40 + misalign;
    for (i = 0; i < 4; i++) bufA[0x40 + misalign + i] = (unsigned char)(V[vi] >> (8 * i));
    memcpy(bufB, bufA, sizeof bufA);

    memset(&start, 0, sizeof start);
    for (i = 0; i < 8; i++) start.r[i] = 0xa5a50000u + i * 0x111;
    start.eflags = flags;
    if (t->has_reg || t->field < 0) set_reg_part(&start, field, t->rsize, rv);   /* before the address registers, which win */
    if (t->p66) insn[n++] = 0x66;
    insn[n++] = t->op[0];
    if (t->nop == 2) insn[n++] = t->op[1];
    if (t->moffs) {
        for (i = 0; i < 4; i++) insn[n++] = (unsigned char)(ea >> (8 * i));
    } else {
        n += build_modrm(insn + n, t->field >= 0 ? (unsigned)t->field : field, s, ea, &start);
    }
    for (i = 0; i < t->immb; i++) insn[n++] = (unsigned char)(imm >> (8 * i));
    len = n;
    memcpy(full, insn, len);
    full[len] = 0xff;
    full[len + 1] = 0x25;
    {
        unsigned cell = (unsigned)(size_t)&tr_ret_cell;
        for (i = 0; i < 4; i++) full[len + 2 + i] = (unsigned char)(cell >> (8 * i));
    }
    memcpy(code_page, full, len + 6);
    real = start;
    tr_run(&real, code_page);
    real = tr_out;

    snprintf(gname, sizeof gname, "decode-%s-reads-length-address-and-sizes", t->group);
    CHECK(gname, port_mirror_decode(insn, len, &start, &in) == 0, "no decode, bytes %02x %02x %02x len %u", insn[0], insn[1], insn[2], len);
    if (port_mirror_decode(insn, len, &start, &in) != 0) return;
    CHECK(gname, in.length == len, "length %u, wanted %u (bytes %02x %02x %02x %02x)", in.length, len, insn[0], insn[1], insn[2], insn[3]);
    CHECK(gname, in.ea == ea, "address 0x%08x, wanted 0x%08x (bytes %02x %02x %02x %02x)", in.ea, ea, insn[0], insn[1], insn[2], insn[3]);
    CHECK(gname, in.msize == t->msize && in.rsize == t->rsize, "sizes %u %u, wanted %u %u (bytes %02x %02x %02x)", in.msize, in.rsize, t->msize, t->rsize, insn[0], insn[1], insn[2]);
    CHECK(gname, in.has_imm == (t->immb != 0), "immediate flag %u (bytes %02x %02x %02x)", in.has_imm, insn[0], insn[1], insn[2]);
    /* a decoder that cuts the instruction short must refuse, never read beyond */
    CHECK(gname, len == 0 || port_mirror_decode(insn, len - 1, &start, &in) != 0, "decoded a truncated instruction (bytes %02x %02x %02x, %u of %u)", insn[0], insn[1], insn[2], len - 1, len);
    if (port_mirror_decode(insn, len, &start, &in) != 0) return;

    emu = start;
    emu.eip = 0x1000;
    port_mirror_exec(&in, &emu, bufB + (in.ea - (unsigned)(size_t)bufA));
    snprintf(gname, sizeof gname, "exec-%s-equals-the-processor", t->group);
    runs++;
    for (i = 0; i < 8; i++)
        CHECK(gname, emu.r[i] == real.r[i], "register %u: port %08x, processor %08x (bytes %02x %02x %02x %02x %02x, mem %08x reg %08x imm %08x flags %03x)",
              i, emu.r[i], real.r[i], insn[0], insn[1], insn[2], insn[3], insn[4], V[vi], rv, imm, flags);
    CHECK(gname, emu.eflags == real.eflags, "flags: port %08x, processor %08x (bytes %02x %02x %02x %02x %02x, mem %08x reg %08x imm %08x flags in %03x)",
          emu.eflags, real.eflags, insn[0], insn[1], insn[2], insn[3], insn[4], V[vi], rv, imm, flags);
    CHECK(gname, memcmp(bufA, bufB, sizeof bufA) == 0, "memory differs (bytes %02x %02x %02x %02x %02x, mem %08x reg %08x imm %08x)", insn[0], insn[1], insn[2], insn[3], insn[4], V[vi], rv, imm);
    CHECK(gname, emu.eip == 0x1000 + len, "eip %08x, wanted %08x", emu.eip, 0x1000 + len);
}

static void run_forms(void)
{
    unsigned t, f, s, v, fi;
    for (t = 0; t < ntmpl; t++) {
        const struct tmpl *m = &tmpls[t];
        unsigned f0 = m->field < 0 ? 0 : (unsigned)m->field, f1 = m->field < 0 ? 8 : (unsigned)m->field + 1;
        for (f = f0; f < f1; f++) {
            if (m->field < 0 && !m->all_shapes && f != 0 && f != 2 && f != 3 && f != 5 && f != 7) continue;
            if (m->moffs && f != 0) continue;
            for (s = 0; s < (m->moffs ? 1u : nshape); s++) {
                if (!m->all_shapes && !m->moffs && (s % 9) != (t % 9)) continue;
                for (v = 0; v < 14; v++)
                    for (fi = 0; fi < 3; fi++) {
                        unsigned rv = fi == 0 ? V[(v * 5 + 3) % 14] : fi == 1 ? V[v] : V[(v + 1) % 14];
                        unsigned imm = V[(v * 3 + 1) % 14];
                        one(m, m->moffs ? 0 : f, &shapes[s], v, rv, imm, FL[(v + fi) & 3], (v + fi) & 1);
                    }
            }
        }
    }
}

/* ---- refusals ---- */

static void refusals(void)
{
    static const struct { const char *what; const char *hex; } bad[] = {
        { "call-through-memory", "ff 15 00 10 00 00" }, { "jmp-through-memory", "ff 24 85 00 10 00 00" }, { "imul", "0f af 05 00 10 00 00" },
        { "neg", "f7 1d 00 10 00 00" }, { "idiv", "f7 3d 00 10 00 00" }, { "mul-byte", "f6 25 00 10 00 00" }, { "shift-left", "c1 25 00 10 00 00 02" },
        { "shift-right-arithmetic", "d1 3d 00 10 00 00" }, { "cmov", "0f 45 05 00 10 00 00" }, { "setcc", "0f 94 05 00 10 00 00" },
        { "inc", "ff 05 00 10 00 00" }, { "dec", "ff 0d 00 10 00 00" }, { "push", "ff 35 00 10 00 00" }, { "rep-movs", "f3 a5" },
        { "movs", "a4" }, { "adc", "11 05 00 10 00 00" }, { "sbb", "19 05 00 10 00 00" }, { "adc-immediate", "83 15 00 10 00 00 01" },
        { "sbb-immediate", "83 1d 00 10 00 00 01" }, { "lea", "8d 05 00 10 00 00" }, { "segment-prefix", "64 8b 05 00 10 00 00" },
        { "lock-prefix", "f0 01 05 00 10 00 00" }, { "address-size-prefix", "67 8b 05 00 10 00 00" }, { "register-operand", "8b c1" },
        { "truncated-displacement", "8b 05 00 10" }, { "truncated-escape", "0f b6" }, { "word-movzx-of-a-word", "66 0f b7 05 00 10 00 00" },
        { "x87", "d9 05 00 10 00 00" }, { "sse", "f2 0f 10 05 00 10 00 00" }, { "group-one-adc", "81 15 00 10 00 00 01 00 00 00" },
        { "not", "f7 15 00 10 00 00" }, { "empty", "" }, { "prefix-only", "66" },
    };
    unsigned i;
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        unsigned char b[16];
        unsigned n = 0;
        const char *p = bad[i].hex;
        struct mirror_regs r;
        struct mirror_insn in;
        char name[96];
        while (*p) {
            unsigned v;
            if (*p == ' ') { p++; continue; }
            sscanf(p, "%2x", &v);
            b[n++] = (unsigned char)v;
            p += 2;
        }
        memset(&r, 0, sizeof r);
        snprintf(name, sizeof name, "decode-refuses-%s", bad[i].what);
        CHECK(name, port_mirror_decode(b, n, &r, &in) == -1, "the bytes %s were decoded", bad[i].hex);
    }
}

static void literals(void)
{
    static const struct { const char *hex; unsigned op, msize, rsize, reg, mem_dst, has_imm, imm, ea, len; } lit[] = {
        /* from the count of the game's objects */
        { "0f b6 81 a7 00 00 00", MI_MOVZX, 1, 4, 0, 0, 0, 0, 0x100 + 0xa7, 7 },
        { "89 82 ac 02 00 00", MI_MOV, 4, 4, 0, 1, 0, 0, 0x200 + 0x2ac, 6 },
        { "c7 44 24 04 00 10 00 00", MI_MOV, 4, 4, 0, 1, 1, 0x1000, 0x300 + 4, 8 },
        { "66 39 5e 1a", MI_CMP, 2, 2, 3, 1, 0, 0, 0x400 + 0x1a, 4 },
        { "a2 39 01 00 00", MI_MOV, 1, 1, 0, 1, 0, 0, 0x139, 5 },
        { "80 05 38 01 00 00 01", MI_ADD, 1, 1, 0, 1, 1, 1, 0x138, 7 },
        { "83 c0 ff", 0, 0, 0, 0, 0, 0, 0, 0, 0 },    /* register form: refused, see below */
        { "83 40 04 ff", MI_ADD, 4, 4, 0, 1, 1, 0xffffffffu, 0x500 + 4, 4 },
        { "66 83 40 04 ff", MI_ADD, 2, 2, 0, 1, 1, 0xffff, 0x500 + 4, 5 },
        { "8b 04 8d 00 10 00 00", MI_MOV, 4, 4, 0, 0, 0, 0, 0x1000 + 4 * 1, 7 },
    };
    unsigned i;
    for (i = 0; i < sizeof lit / sizeof lit[0]; i++) {
        unsigned char b[16];
        unsigned n = 0;
        const char *p = lit[i].hex;
        struct mirror_regs r;
        struct mirror_insn in;
        int rc;
        while (*p) {
            unsigned v;
            if (*p == ' ') { p++; continue; }
            sscanf(p, "%2x", &v);
            b[n++] = (unsigned char)v;
            p += 2;
        }
        memset(&r, 0, sizeof r);
        r.r[1] = 0x100; r.r[2] = 0x200; r.r[6] = 0x400; r.r[0] = 0x500; r.r[7] = 0x300;
        if (i == 2) { r.r[4] = 0x300; }
        if (i == 9) r.r[1] = 1;
        if (i == 6) {
            CHECK("decode-literals-from-the-count", port_mirror_decode(b, n, &r, &in) == -1, "register form decoded");
            continue;
        }
        rc = port_mirror_decode(b, n, &r, &in);
        CHECK("decode-literals-from-the-count", rc == 0, "no decode of %s", lit[i].hex);
        if (rc) continue;
        CHECK("decode-literals-from-the-count", in.op == lit[i].op && in.msize == lit[i].msize && in.rsize == lit[i].rsize && in.reg == lit[i].reg &&
              in.mem_dst == lit[i].mem_dst && in.has_imm == lit[i].has_imm && in.imm == lit[i].imm && in.length == lit[i].len,
              "%s: op %u msize %u rsize %u reg %u dst %u imm %u/%08x len %u", lit[i].hex, in.op, in.msize, in.rsize, in.reg, in.mem_dst, in.has_imm, in.imm, in.length);
        if (i == 0) CHECK("decode-literals-from-the-count", in.ea == 0x100 + 0xa7, "ea %08x", in.ea);
        if (i == 1) CHECK("decode-literals-from-the-count", in.ea == 0x200 + 0x2ac, "ea %08x", in.ea);
        if (i == 2) CHECK("decode-literals-from-the-count", in.ea == 0x304, "ea %08x", in.ea);
        if (i == 3) CHECK("decode-literals-from-the-count", in.ea == 0x400 + 0x1a, "ea %08x", in.ea);
        if (i == 4 || i == 5) CHECK("decode-literals-from-the-count", in.ea == lit[i].ea, "ea %08x", in.ea);
        if (i == 9) CHECK("decode-literals-from-the-count", in.ea == 0x1004, "ea %08x", in.ea);
    }
}

/* ---- decision, scan, hex ---- */

static int fake_kind;
static int fake_query(unsigned addr, int *accessible, unsigned *next)
{
    if (fake_kind == 3) return -1;
    *accessible = (fake_kind == 1 && addr >= 0x8000 && addr < 0x9000) || (fake_kind == 5 && addr >= 0x11000 && addr < 0x12000) ||
                  (fake_kind == 6 && addr >= 0x1ff000 && addr < 0x200000) || (fake_kind == 7 && addr >= 0x10000 && addr < 0x11000) ||
                  (fake_kind == 8 && addr >= 0x200000 && addr < 0x201000) || (fake_kind == 9 && addr >= 0x10000 && addr < 0x12000) ||
                  (fake_kind == 10 && addr >= 0x10000 && addr < 0x30000);
    *next = fake_kind == 2 ? 0 : (addr & ~0xfffu) + 0x1000;
    if (fake_kind == 4) *next = 0x200000;   /* one region for the whole range */
    if (fake_kind == 10 && addr >= 0x10000 && addr < 0x30000) *next = 0x30000;   /* one accessible region from the header page on */
    return 0;
}

static unsigned char hdr_buf[0x400];

/* An invented header: base, then `.hole` at 0x11000 of `hole_size` bytes (raw size `raw`), then a section at `next_rva`. */
static void make_header(unsigned base, const char *name, unsigned hole_rva, unsigned hole_size, unsigned raw, unsigned next_rva)
{
    unsigned char *p = hdr_buf;
    unsigned pe = 0x80, sec = pe + 24 + 224;
    memset(hdr_buf, 0, sizeof hdr_buf);
    p[0] = 'M'; p[1] = 'Z'; p[0x3c] = (unsigned char)pe;
    memcpy(p + pe, "PE\0\0", 4);
    p[pe + 6] = 2;                  /* sections */
    p[pe + 20] = 224;               /* optional header size */
    p[pe + 24] = 0x0b; p[pe + 25] = 0x01;
    memcpy(p + pe + 24 + 28, &base, 4);
    memcpy(p + sec, name, strlen(name));
    memcpy(p + sec + 8, &hole_size, 4);
    memcpy(p + sec + 12, &hole_rva, 4);
    memcpy(p + sec + 16, &raw, 4);
    memcpy(p + sec + 40, ".text", 5);
    memcpy(p + sec + 40 + 12, &next_rva, 4);
}

static void image_hole(void)
{
    const char *G = "the-image-hole-is-read-from-the-header-and-only-the-right-one-is-accepted";
    unsigned end = 0;
    make_header(0x10000, ".hole", 0x1000, 0x1ef000, 0, 0x1f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == 0 && end == 0x200000, "the good header: end %08x", end);
    make_header(0x10000, ".hole", 0x1000, 0x1ef000, 0, 0x2f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "a gap after the filler");
    make_header(0x10000, ".hole", 0x1000, 0x2ef000, 0, 0x2f0000);
    end = 0;
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == 0 && end == 0x300000, "a larger filler: end %08x", end);
    make_header(0x10000, ".hole", 0x1000, 0x1ee000, 0, 0x1ef000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "the next section below 0x200000");
    make_header(0x400000, ".hole", 0x1000, 0x1ef000, 0, 0x1f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "image base 0x400000");
    make_header(0x20000, ".hole", 0x1000, 0x1ef000, 0, 0x1f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "image base 0x20000");
    make_header(0x10000, ".text", 0x1000, 0x1ef000, 0, 0x1f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "the first section is not the filler");
    make_header(0x10000, ".hole", 0x2000, 0x1ef000, 0, 0x1f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "the filler does not begin at 0x11000");
    make_header(0x10000, ".hole", 0x1000, 0x1ef000, 0x200, 0x1f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "the filler has bytes in the file");
    make_header(0x10000, ".hole", 0x1000, 0x1ef000, 0, 0x1f0000);
    hdr_buf[0] = 'X';
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "no MZ");
    make_header(0x10000, ".hole", 0x1000, 0x1ef000, 0, 0x1f0000);
    hdr_buf[0x80] = 'Q';
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "no PE signature");
    make_header(0x10000, ".hole", 0x1000, 0x1ef000, 0, 0x1f0000);
    CHECK(G, port_mirror_image_hole(hdr_buf, 0x80 + 24 + 224 + 79, &end) == -1, "a buffer that ends inside the section table");
    CHECK(G, port_mirror_image_hole(hdr_buf, 0x30, &end) == -1, "a buffer shorter than the DOS header");
    make_header(0x10000, ".hole", 0x1000, 0x1ef000, 0, 0x1f0000);
    hdr_buf[0x3c] = 0xff; hdr_buf[0x3d] = 0xff;
    CHECK(G, port_mirror_image_hole(hdr_buf, sizeof hdr_buf, &end) == -1, "a PE offset past the buffer");
}

static void decision_scan_hex(void)
{
    const char *G = "decision-serves-only-reads-and-writes-wholly-inside-the-first-2-mb-but-not-the-header-page-of-the-game-thread-in-game-code";
    const char *S = "start-check-accepts-an-address-space-with-nothing-accessible-in-the-first-2-mb";
    char err[200], hex[64];
    static const unsigned char b[3] = { 0x88, 0x00, 0xff };
    CHECK(G, port_mirror_decision(0, 0, 1, 1, 1) == 1, "read of 0");
    CHECK(G, port_mirror_decision(1, 0, 4, 1, 1) == 1, "write of 0, 4 bytes");
    CHECK(G, port_mirror_decision(0, 0xffff, 1, 1, 1) == 1, "last byte 0xffff");
    CHECK(G, port_mirror_decision(0, 0xfffc, 4, 1, 1) == 1, "last four bytes of the first 64 KB");
    CHECK(G, port_mirror_decision(0, 0xffff, 1, 1, 1) == 1, "0xffff, the byte before the header page");
    CHECK(G, port_mirror_decision(0, 0xfffc, 4, 1, 1) == 1, "0xfffc, four bytes, the last word before the header page");
    CHECK(G, port_mirror_decision(0, 0xfffe, 4, 1, 1) == 0, "four bytes from 0xfffe reach the header page");
    CHECK(G, port_mirror_decision(0, 0xffff, 2, 1, 1) == 0, "two bytes from 0xffff reach the header page");
    CHECK(G, port_mirror_decision(0, 0x10000, 1, 1, 1) == 0, "0x10000, the header page");
    CHECK(G, port_mirror_decision(1, 0x10fff, 1, 1, 1) == 0, "0x10fff, the header page");
    CHECK(G, port_mirror_decision(1, 0x10ffc, 4, 1, 1) == 0, "0x10ffc, the header page");
    CHECK(G, port_mirror_decision(0, 0x10ffe, 4, 1, 1) == 0, "four bytes from 0x10ffe reach past the header page");
    CHECK(G, port_mirror_decision(0, 0x11000, 4, 1, 1) == 1, "0x11000, the end of the header page");
    CHECK(G, port_mirror_decision(1, 0x11000, 1, 1, 1) == 1, "write at 0x11000");
    CHECK(G, port_mirror_decision(0, 0x3e0cc, 4, 1, 1) == 1, "0x3e0cc");
    CHECK(G, port_mirror_decision(0, 0x1fffff, 1, 1, 1) == 1, "last byte 0x1fffff");
    CHECK(G, port_mirror_decision(1, 0x1ffffc, 4, 1, 1) == 1, "last word of the copy");
    CHECK(G, port_mirror_decision(0, 0x1fffff, 2, 1, 1) == 0, "straddle from 0x1fffff");
    CHECK(G, port_mirror_decision(1, 0x1ffffe, 4, 1, 1) == 0, "straddle from 0x1ffffe");
    CHECK(G, port_mirror_decision(0, 0x1ffffd, 4, 1, 1) == 0, "straddle from 0x1ffffd");
    CHECK(G, port_mirror_decision(0, 0x200000, 1, 1, 1) == 0, "0x200000");
    CHECK(G, port_mirror_decision(0, 0x200001, 4, 1, 1) == 0, "0x200001");
    CHECK(G, port_mirror_decision(0, 0xffffffffu, 2, 1, 1) == 0, "the top of the address space (address plus size overflows)");
    CHECK(G, port_mirror_decision(0, 0xfffffff0u, 0x20, 1, 1) == 0, "an access that wraps");
    CHECK(G, port_mirror_decision(0, 0x1000, 0, 1, 1) == 0, "size 0");
    CHECK(G, port_mirror_decision(8, 0x1000, 1, 1, 1) == 0, "execute fault");
    CHECK(G, port_mirror_decision(8, 0x20000, 1, 1, 1) == 0, "execute fault above 64 KB");
    CHECK(G, port_mirror_decision(2, 0x1000, 1, 1, 1) == 0, "an unknown kind");
    CHECK(G, port_mirror_decision(0, 0x1000, 1, 0, 1) == 0, "another thread");
    CHECK(G, port_mirror_decision(1, 0x1000, 1, 1, 0) == 0, "code that is not the game's");
    CHECK(G, port_mirror_decision(1, 0x80000, 1, 1, 0) == 0, "code that is not the game's, above 64 KB");

    fake_kind = 0;
    CHECK(S, port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == 0, "refused: %s", err);
    fake_kind = 4;
    CHECK(S, port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == 0, "one region: %s", err);
    fake_kind = 1;
    CHECK("start-check-refuses-an-accessible-page-and-names-its-address", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == -1 && strstr(err, "0x00008000") && strncmp(err, "mirror: ", 8) == 0, "got: %s", err);
    fake_kind = 5;
    CHECK("start-check-refuses-an-accessible-page-above-64-kb-and-names-its-address", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == -1 && strstr(err, "0x00011000"), "got: %s", err);
    fake_kind = 6;
    CHECK("start-check-refuses-an-accessible-page-at-the-end-of-the-copy-and-names-its-address", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == -1 && strstr(err, "0x001ff000"), "got: %s", err);
    fake_kind = 7;
    CHECK("start-check-lets-the-header-page-through-when-told-to", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == 0, "got: %s", err);
    CHECK("start-check-refuses-an-accessible-header-page-when-not-told-to-and-names-its-address", port_mirror_scan(fake_query, 0, 0, err, sizeof err) == -1 && strstr(err, "0x00010000"), "got: %s", err);
    fake_kind = 9;
    CHECK("start-check-lets-only-the-header-page-through", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == -1 && strstr(err, "0x00011000"), "got: %s", err);
    fake_kind = 10;
    CHECK("start-check-lets-only-the-header-page-through-of-one-big-region", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == -1 && strstr(err, "0x00011000"), "got: %s", err);
    fake_kind = 8;
    CHECK("start-check-does-not-look-at-0x200000-and-above", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == 0, "got: %s", err);
    fake_kind = 3;
    CHECK("start-check-refuses-when-the-system-cannot-answer", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == -1 && strstr(err, "0x00000000"), "got: %s", err);
    fake_kind = 2;
    CHECK("start-check-ends-when-the-system-gives-no-progress", port_mirror_scan(fake_query, 0x10000, 0x11000, err, sizeof err) == 0, "got: %s", err);

    port_mirror_hex(b, 3, hex, sizeof hex);
    CHECK("hex-prints-bytes-separated-by-a-blank", strcmp(hex, "88 00 ff") == 0, "got '%s'", hex);
    port_mirror_hex(b, 3, hex, 6);
    CHECK("hex-never-writes-past-the-buffer", strlen(hex) < 6, "got '%s'", hex);
}

int main(void)
{
    unsigned i;
    code_page = VirtualAlloc(NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!code_page) {
        printf("FAIL native-setup: no executable page\n");
        return 1;
    }
    build_templates();
    build_shapes();
    decision_scan_hex();
    image_hole();
    refusals();
    literals();
    run_forms();
    for (i = 0; i < ngroups; i++) {
        if (groups[i].bad) printf("FAIL %s: %s\n", groups[i].name, groups[i].first);
        else printf("ok %s (%u checks)\n", groups[i].name, groups[i].n);
    }
    printf("native: %llu instruction runs on the processor and on the port, %u templates, %u addressing shapes\n", runs, ntmpl, nshape);
    return 0;
}
