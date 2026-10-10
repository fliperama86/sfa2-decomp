/* The PS1's copy of RAM at address 0, served by the fault handler (mirror.c) with the decisions and the
 * instruction work in plain functions (mirrorcore.c) that need no running program and no system header.
 *
 * [0, MIRROR_LIMIT) is served, but for the image's header page: below MIRROR_IMAGE_BASE Windows never gives a
 * process memory, and from MIRROR_HOLE to MIRROR_LIMIT the program's own image holds the range (the link settings
 * are in hostbuild.py), closed at start, so every access of the game there faults. */
#ifndef PORT_MIRROR_H
#define PORT_MIRROR_H

#include <stddef.h>

#define MIRROR_LIMIT 0x200000u      /* the console's RAM, 2 MB, seen a second time from address 0 */
#define MIRROR_IMAGE_BASE 0x10000u  /* where the program's image is linked: the first address a process can have */
#define MIRROR_HOLE 0x11000u        /* where the image's filler section begins, after the header's page */
/* The image's header page, [MIRROR_IMAGE_BASE, MIRROR_HOLE), stays readable (the C library reads the header at
 * exit), so an access there is not served: a read returns the header's bytes without a fault, a write faults on
 * the read-only page and is the crash line. */

/* The general registers in the processor's order, then the instruction pointer and the flags. */
struct mirror_regs {
    unsigned r[8];   /* eax ecx edx ebx esp ebp esi edi */
    unsigned eip, eflags;
};

enum { MI_MOV, MI_MOVZX, MI_MOVSX, MI_ADD, MI_OR, MI_AND, MI_SUB, MI_XOR, MI_CMP, MI_TEST };

/* One decoded instruction that has a memory operand. */
struct mirror_insn {
    unsigned char op;       /* MI_* */
    unsigned char msize;    /* bytes of the memory operand: 1, 2 or 4 */
    unsigned char rsize;    /* bytes of the register operand (or of the destination for MOVZX/MOVSX) */
    unsigned char mem_dst;  /* 1: the memory operand is the destination */
    unsigned char has_imm;  /* 1: the other operand is the immediate `imm` (no register operand) */
    unsigned char reg;      /* the register operand's number (for 1 byte: 4..7 are ah ch dh bh) */
    unsigned imm;           /* the immediate, sign-extended to 32 bits */
    unsigned ea;            /* the memory operand's address, from the registers before the instruction */
    unsigned length;        /* bytes of the instruction */
};

/* Is this fault one the mirror serves? kind: 0 read, 1 write, 8 execute (the access violation's own
 * numbers); addr and size: the access; game_thread: the thread is the game's; in_game_code: the
 * instruction pointer is in the game's own compiled code. 1: serve. 0: not ours (the crash line):
 * wrong kind, wrong thread, wrong code, an access outside [0, MIRROR_LIMIT) or one that begins inside it
 * and ends at or past MIRROR_LIMIT, one that touches the header page. */
int port_mirror_decision(unsigned kind, unsigned addr, unsigned size, int game_thread, int in_game_code);

/* Decode the instruction at `code` (`avail` bytes readable, at most 15 are looked at) against the
 * registers before it. 0 and *out filled; -1 if it is not one of the forms served (the list is in the
 * file; a `call` or `jmp` through memory, imul, idiv, mul, neg, shifts, cmovcc, setcc, inc, dec, push,
 * string instructions, prefixes other than 0x66 are not). */
int port_mirror_decode(const unsigned char *code, unsigned avail, const struct mirror_regs *regs, struct mirror_insn *out);

/* Carry the instruction out: the memory operand is at `mem` (the mapped RAM), the registers and flags
 * are changed as the processor would, and eip moves past the instruction. */
void port_mirror_exec(const struct mirror_insn *insn, struct mirror_regs *regs, void *mem);

/* The status flags (CF PF AF ZF SF OF) of `a op b` computed by the processor itself, at `size` bytes;
 * the result in *res (cmp and test: the value the operation computes, which is not stored). */
unsigned port_mirror_alu(int op, unsigned size, unsigned a, unsigned b, unsigned *res);

/* "88 00 ..." for up to `n` bytes, into `out`. */
void port_mirror_hex(const unsigned char *bytes, unsigned n, char *out, size_t outsize);

/* Does the image whose header is at `hdr` (`size` bytes readable) carry the filler section that holds the range?
 * 0 and *end (the address where the filler ends, MIRROR_LIMIT or above): the header says image base
 * MIRROR_IMAGE_BASE and its first section is `.hole`, uninitialized, at MIRROR_HOLE, followed with no gap by
 * the next section. -1 when not (the image is linked the old way, or the header is not a valid one). */
int port_mirror_image_hole(const unsigned char *hdr, unsigned size, unsigned *end);

/* The start check: `query(addr, &accessible, &next)` says whether the page at `addr` can be read or written
 * and where the next region begins. -1 with a line when a page of [0, MIRROR_LIMIT) is accessible, except
 * the pages of [skip_begin, skip_end) (the image's header page, when the image is the program's own; 0, 0
 * for none). */
typedef int (*port_mirror_query)(unsigned addr, int *accessible, unsigned *next);
int port_mirror_scan(port_mirror_query query, unsigned skip_begin, unsigned skip_end, char *err, size_t errsize);

#endif
