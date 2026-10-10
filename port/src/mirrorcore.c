/* The decisions and the instruction work of the PS1's copy of RAM at address 0 (see mirror.h and mirror.c).
 *
 * The forms served are the ones the count of the game's objects shows for loads, stores and plain
 * arithmetic (operand size 8, 16 or 32 bits, 0x66 the only prefix):
 *   mov     88 89 8a 8b, c6 /0 c7 /0, a0 a1 a2 a3 (absolute address)
 *   movzx   0f b6 b7        movsx  0f be bf
 *   add or and sub xor cmp   00-3b (low two bits 0..3 of the opcode) and 80 81 83 /0 /1 /4 /5 /6 /7
 *   test    84 85, f6 /0 f7 /0
 * Everything else, including adc, sbb, inc, dec, push, call and jmp through memory, imul, idiv, mul,
 * neg, shifts, cmovcc, setcc and the string instructions, is not served: the decoder says so.
 * The flags of an arithmetic form come from the processor: the operation is run on registers by the
 * small functions below, and only the six status flags are taken over. */
#include "mirror.h"

#include <stdio.h>
#include <string.h>

#define STATUS 0x8d5u   /* CF PF AF ZF SF OF */

int port_mirror_decision(unsigned kind, unsigned addr, unsigned size, int game_thread, int in_game_code)
{
    if (kind != 0 && kind != 1) return 0;
    if (!game_thread || !in_game_code) return 0;
    if (size == 0 || addr >= MIRROR_LIMIT || size > MIRROR_LIMIT - addr) return 0;
    if (addr < MIRROR_HOLE && addr + size > MIRROR_IMAGE_BASE) return 0;   /* touches the image's header page */
    return 1;
}

/* ---- the decoder ---- */

static unsigned sx8(unsigned v) { return (unsigned)(int)(signed char)v; }

/* ModRM, SIB and displacement at p[*pos]; fills ea, reg field. -1 for a register operand or a short buffer. */
static int modrm(const unsigned char *p, unsigned n, unsigned *pos, const struct mirror_regs *r, unsigned *ea, unsigned *field)
{
    unsigned m, mod, rm, a = 0, i;
    if (*pos >= n) return -1;
    m = p[(*pos)++];
    mod = m >> 6;
    rm = m & 7;
    *field = (m >> 3) & 7;
    if (mod == 3) return -1;
    if (rm == 4) {
        unsigned sib, base, idx;
        if (*pos >= n) return -1;
        sib = p[(*pos)++];
        base = sib & 7;
        idx = (sib >> 3) & 7;
        if (idx != 4) a += r->r[idx] << (sib >> 6);
        if (base == 5 && mod == 0) {
            if (*pos + 4 > n) return -1;
            for (i = 0; i < 4; i++) a += (unsigned)p[*pos + i] << (8 * i);
            *pos += 4;
            *ea = a;
            return 0;
        }
        a += r->r[base];
    } else if (rm == 5 && mod == 0) {
        if (*pos + 4 > n) return -1;
        for (i = 0; i < 4; i++) a += (unsigned)p[*pos + i] << (8 * i);
        *pos += 4;
        *ea = a;
        return 0;
    } else {
        a += r->r[rm];
    }
    if (mod == 1) {
        if (*pos >= n) return -1;
        a += sx8(p[(*pos)++]);
    } else if (mod == 2) {
        if (*pos + 4 > n) return -1;
        for (i = 0; i < 4; i++) a += (unsigned)p[*pos + i] << (8 * i);
        *pos += 4;
    }
    *ea = a;
    return 0;
}

static int imm(const unsigned char *p, unsigned n, unsigned *pos, unsigned bytes, int sign, unsigned *v)
{
    unsigned i, x = 0;
    if (*pos + bytes > n) return -1;
    for (i = 0; i < bytes; i++) x |= (unsigned)p[*pos + i] << (8 * i);
    *pos += bytes;
    if (sign && bytes == 1) x = sx8(x);
    *v = x;
    return 0;
}

/* the arithmetic operation of the opcode's group field / the opcode's bits 3..5; -1 for adc and sbb */
static int alu_op(unsigned f)
{
    switch (f) {
    case 0: return MI_ADD;
    case 1: return MI_OR;
    case 4: return MI_AND;
    case 5: return MI_SUB;
    case 6: return MI_XOR;
    case 7: return MI_CMP;
    }
    return -1;
}

int port_mirror_decode(const unsigned char *code, unsigned avail, const struct mirror_regs *regs, struct mirror_insn *out)
{
    unsigned n = avail > 15 ? 15 : avail, pos = 0, opc, field = 0, osz = 4, size, v = 0;
    struct mirror_insn o;

    memset(&o, 0, sizeof o);
    if (pos < n && code[pos] == 0x66) {
        osz = 2;
        pos++;
    }
    if (pos >= n) return -1;
    opc = code[pos++];
    if (opc == 0x0f) {
        unsigned sub;
        if (pos >= n) return -1;
        sub = code[pos++];
        if (sub != 0xb6 && sub != 0xb7 && sub != 0xbe && sub != 0xbf) return -1;
        o.op = (sub & 8) ? MI_MOVSX : MI_MOVZX;
        o.msize = (sub & 1) ? 2 : 1;
        o.rsize = (unsigned char)osz;
        if (o.msize == 2 && osz == 2) return -1;   /* no such instruction */
        if (modrm(code, n, &pos, regs, &o.ea, &field) != 0) return -1;
        o.reg = (unsigned char)field;
    } else if (opc >= 0xa0 && opc <= 0xa3) {
        unsigned i;
        size = (opc & 1) ? osz : 1;
        if (pos + 4 > n) return -1;
        for (i = 0; i < 4; i++) o.ea |= (unsigned)code[pos + i] << (8 * i);
        pos += 4;
        o.op = MI_MOV;
        o.msize = o.rsize = (unsigned char)size;
        o.mem_dst = (opc & 2) ? 1 : 0;
        o.reg = 0;
    } else if (opc < 0x40 && (opc & 7) < 4 && alu_op((opc >> 3) & 7) >= 0) {
        int op = alu_op((opc >> 3) & 7);
        size = (opc & 1) ? osz : 1;
        if (modrm(code, n, &pos, regs, &o.ea, &field) != 0) return -1;
        o.op = (unsigned char)op;
        o.msize = o.rsize = (unsigned char)size;
        o.mem_dst = (opc & 2) ? 0 : 1;
        o.reg = (unsigned char)field;
    } else if (opc == 0x84 || opc == 0x85) {
        size = (opc & 1) ? osz : 1;
        if (modrm(code, n, &pos, regs, &o.ea, &field) != 0) return -1;
        o.op = MI_TEST;
        o.msize = o.rsize = (unsigned char)size;
        o.mem_dst = 1;
        o.reg = (unsigned char)field;
    } else if (opc >= 0x88 && opc <= 0x8b) {
        size = (opc & 1) ? osz : 1;
        if (modrm(code, n, &pos, regs, &o.ea, &field) != 0) return -1;
        o.op = MI_MOV;
        o.msize = o.rsize = (unsigned char)size;
        o.mem_dst = (opc & 2) ? 0 : 1;
        o.reg = (unsigned char)field;
    } else if (opc == 0x80 || opc == 0x81 || opc == 0x83) {
        int op;
        size = (opc == 0x80) ? 1 : osz;
        if (modrm(code, n, &pos, regs, &o.ea, &field) != 0) return -1;
        op = alu_op(field);
        if (op < 0) return -1;
        if (imm(code, n, &pos, opc == 0x81 ? size : 1, opc != 0x81, &v) != 0) return -1;
        o.op = (unsigned char)op;
        o.msize = o.rsize = (unsigned char)size;
        o.mem_dst = 1;
        o.has_imm = 1;
        o.imm = v;
    } else if (opc == 0xf6 || opc == 0xf7 || opc == 0xc6 || opc == 0xc7) {
        size = (opc & 1) ? osz : 1;
        if (modrm(code, n, &pos, regs, &o.ea, &field) != 0) return -1;
        if (field != 0) return -1;   /* f6/f7 /1..7 (not, neg, mul, imul, div, idiv) and c6/c7 /1..7 are not served */
        if (imm(code, n, &pos, size, 0, &v) != 0) return -1;
        o.op = (opc & 0xfe) == 0xc6 ? MI_MOV : MI_TEST;
        o.msize = o.rsize = (unsigned char)size;
        o.mem_dst = 1;
        o.has_imm = 1;
        o.imm = v;
    } else {
        return -1;
    }
    if (o.has_imm && o.msize < 4) o.imm &= (1u << (8 * o.msize)) - 1u;
    o.length = pos;
    *out = o;
    return 0;
}

/* ---- the operation ---- */

#define ALU8(NAME, INS) \
    static unsigned NAME##8(unsigned a, unsigned b, unsigned *res) { unsigned char x = (unsigned char)a, y = (unsigned char)b; unsigned f; \
        __asm__ volatile(INS "b %2, %0\n\tpushfl\n\tpopl %1" : "+q"(x), "=r"(f) : "q"(y) : "cc", "memory"); *res = x; return f & STATUS; }
#define ALU16(NAME, INS) \
    static unsigned NAME##16(unsigned a, unsigned b, unsigned *res) { unsigned short x = (unsigned short)a, y = (unsigned short)b; unsigned f; \
        __asm__ volatile(INS "w %2, %0\n\tpushfl\n\tpopl %1" : "+r"(x), "=r"(f) : "r"(y) : "cc", "memory"); *res = x; return f & STATUS; }
#define ALU32(NAME, INS) \
    static unsigned NAME##32(unsigned a, unsigned b, unsigned *res) { unsigned x = a, y = b, f; \
        __asm__ volatile(INS "l %2, %0\n\tpushfl\n\tpopl %1" : "+r"(x), "=r"(f) : "r"(y) : "cc", "memory"); *res = x; return f & STATUS; }
#define ALU(NAME, INS) ALU8(NAME, INS) ALU16(NAME, INS) ALU32(NAME, INS)

ALU(op_add, "add")
ALU(op_or, "or")
ALU(op_and, "and")
ALU(op_sub, "sub")
ALU(op_xor, "xor")
ALU(op_cmp, "cmp")
ALU(op_test, "test")

unsigned port_mirror_alu(int op, unsigned size, unsigned a, unsigned b, unsigned *res)
{
    typedef unsigned (*fn)(unsigned, unsigned, unsigned *);
    static const fn table[10][3] = {
        { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
        { op_add8, op_add16, op_add32 }, { op_or8, op_or16, op_or32 }, { op_and8, op_and16, op_and32 },
        { op_sub8, op_sub16, op_sub32 }, { op_xor8, op_xor16, op_xor32 }, { op_cmp8, op_cmp16, op_cmp32 },
        { op_test8, op_test16, op_test32 },
    };
    return table[op][size == 1 ? 0 : size == 2 ? 1 : 2](a, b, res);
}

static unsigned get_reg(const struct mirror_regs *r, unsigned num, unsigned size)
{
    if (size == 1) return num < 4 ? r->r[num] & 0xffu : (r->r[num - 4] >> 8) & 0xffu;
    if (size == 2) return r->r[num] & 0xffffu;
    return r->r[num];
}

static void set_reg(struct mirror_regs *r, unsigned num, unsigned size, unsigned v)
{
    if (size == 1) {
        if (num < 4) r->r[num] = (r->r[num] & ~0xffu) | (v & 0xffu);
        else r->r[num - 4] = (r->r[num - 4] & ~0xff00u) | ((v & 0xffu) << 8);
    } else if (size == 2) {
        r->r[num] = (r->r[num] & ~0xffffu) | (v & 0xffffu);
    } else {
        r->r[num] = v;
    }
}

static unsigned load(const void *mem, unsigned size)
{
    const unsigned char *p = (const unsigned char *)mem;
    unsigned i, v = 0;
    for (i = 0; i < size; i++) v |= (unsigned)p[i] << (8 * i);
    return v;
}

static void store(void *mem, unsigned size, unsigned v)
{
    unsigned char *p = (unsigned char *)mem;
    unsigned i;
    for (i = 0; i < size; i++) p[i] = (unsigned char)(v >> (8 * i));
}

void port_mirror_exec(const struct mirror_insn *in, struct mirror_regs *r, void *mem)
{
    if (in->op == MI_MOV) {
        if (in->mem_dst) store(mem, in->msize, in->has_imm ? in->imm : get_reg(r, in->reg, in->rsize));
        else set_reg(r, in->reg, in->rsize, load(mem, in->msize));
    } else if (in->op == MI_MOVZX || in->op == MI_MOVSX) {
        unsigned v = load(mem, in->msize);
        if (in->op == MI_MOVSX) v = in->msize == 1 ? (unsigned)(int)(signed char)v : (unsigned)(int)(short)v;
        set_reg(r, in->reg, in->rsize, v);
    } else {
        unsigned m = load(mem, in->msize), other = in->has_imm ? in->imm : get_reg(r, in->reg, in->rsize), res, fl;
        unsigned a = (in->mem_dst || in->has_imm) ? m : other, b = (in->mem_dst || in->has_imm) ? other : m;
        fl = port_mirror_alu(in->op, in->msize, a, b, &res);
        if (in->op != MI_CMP && in->op != MI_TEST) {
            if (in->mem_dst) store(mem, in->msize, res);
            else set_reg(r, in->reg, in->rsize, res);
        }
        r->eflags = (r->eflags & ~STATUS) | fl;
    }
    r->eip += in->length;
}

void port_mirror_hex(const unsigned char *bytes, unsigned n, char *out, size_t outsize)
{
    unsigned i;
    size_t at = 0;
    out[0] = 0;
    for (i = 0; i < n && at + 4 < outsize; i++)
        at += (size_t)snprintf(out + at, outsize - at, i ? " %02x" : "%02x", bytes[i]);
}

int port_mirror_scan(port_mirror_query query, unsigned skip_begin, unsigned skip_end, char *err, size_t errsize)
{
    unsigned a = 0;
    while (a < MIRROR_LIMIT) {
        int accessible = 0;
        unsigned next = 0;
        if (query(a, &accessible, &next) != 0) {
            snprintf(err, errsize, "mirror: the system cannot say what is at 0x%08x; the PS1's copy of RAM at address 0 cannot be relied on", a);
            return -1;
        }
        if (accessible && a >= skip_begin && a < skip_end) {
            if (next > skip_end) next = skip_end;   /* only the pages of the skipped window are let through */
        } else if (accessible) {
            snprintf(err, errsize, "mirror: the system has an accessible page at 0x%08x; the PS1's copy of RAM at address 0 cannot be served here", a);
            return -1;
        }
        if (next <= a) next = a + 0x1000;
        a = next;
    }
    return 0;
}

/* ---- the image's filler section ---- */

static unsigned le16(const unsigned char *p) { return p[0] | (unsigned)p[1] << 8; }
static unsigned le32(const unsigned char *p) { return le16(p) | (unsigned)le16(p + 2) << 16; }

int port_mirror_image_hole(const unsigned char *hdr, unsigned size, unsigned *end)
{
    unsigned pe, sections, optsize, table, first, second, hole_va, next_va;
    if (size < 0x40 || hdr[0] != 'M' || hdr[1] != 'Z') return -1;
    pe = le32(hdr + 0x3c);
    if (pe > size || size - pe < 24 + 96) return -1;
    if (memcmp(hdr + pe, "PE\0\0", 4) != 0) return -1;
    sections = le16(hdr + pe + 6);
    optsize = le16(hdr + pe + 20);
    if (sections < 2 || optsize < 96 || le16(hdr + pe + 24) != 0x10b) return -1;
    if (le32(hdr + pe + 24 + 28) != MIRROR_IMAGE_BASE) return -1;
    table = pe + 24 + optsize;
    if (table > size || size - table < 80) return -1;
    first = table;
    second = table + 40;
    if (memcmp(hdr + first, ".hole\0\0\0", 8) != 0) return -1;
    hole_va = MIRROR_IMAGE_BASE + le32(hdr + first + 12);
    if (hole_va != MIRROR_HOLE) return -1;
    if (le32(hdr + first + 16) != 0) return -1;   /* uninitialized: nothing of the file is in it */
    next_va = MIRROR_IMAGE_BASE + le32(hdr + second + 12);
    if (next_va < MIRROR_LIMIT) return -1;
    if (next_va - hole_va != ((le32(hdr + first + 8) + 0xfffu) & ~0xfffu)) return -1;   /* no gap between the filler and the next section */
    *end = next_va;
    return 0;
}
