/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred, not an original name): the sibling of
   func_80140b5c. It makes ref_other point at the object's partner
   (object->other) and computes a byte in the partner's field_64 from the
   same tables, in the same way, except that a value of 0x80 or more is set
   to 0x7f (func_80140b5c clears it when bit 7 is set), there is no early
   return for a == -1, and the sum of the random number and b << 5 is
   added, not or-ed. After the handler call (func_80140ec0) it returns 0 at
   once when either of two configuration bytes (fields 4d, 4e) is set.
   Otherwise it takes the byte (at least 1), records the low five bits of a
   in object->field_be and 0xff in field_bf, calls func_80155d4c with a
   table byte selected by those five bits and the object's side, subtracts
   the byte from the partner's 16-bit health, clears health when it went
   negative and bit 15 of a is set, and returns 1 when health is negative,
   else 0. The tree declares the second argument as an int; it is narrowed
   to 16 bits at entry. */
int func_80140cd8(Object *object, int arg_a, int b) {
    u16 a = arg_a;
    s16 idx;
    s8 slot;
    int t;
    Config *config;

    ref_other.p = object->other;
    ref_other.p->field_64 = table_801777d4[object->field_cf + ((a & 0x1f) << 5)];
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 5 instruction slots. */
    do {
        t = get_random_byte() & 0x1f;
    } while (0);
    b <<= 5;
    t += b;
    idx = t;
    ref_other.p->field_64 += table_80177bd4[idx] - 16;
    t = table_80179834[object->kind];
    game_state.cursor = (u32 *)table_8017984c[t];
    b = ref_other.p->field_64;
    /* Stand-in, not recovered source: the test below, whose condition is 0 for every value of b, reproduces the original's choice of registers. Without it this function differs from the original in 2 instruction slots. */
    /* The original loads the byte into the saved register and masks it for the compare (`lbu s0`, `andi v0,s0,0xff`). Without a branch between the load and the compare the compiler knows the loaded value's width and drops the mask (one instruction less). The test is that branch while the compiler joins instructions and is gone afterwards: it leaves no instruction, and its body is never run. This reading is inferred. */
    if (((u32)b << 24) & 0x100) {
        b = 0;
    }
    if ((u8)b >= 0x80) {
        b = 0x7f;
    }
    ref_other.p->field_64 = b + ((u8 *)game_state.cursor)[(u8)b];
    func_80140ec0();
    config = game_state.config;
    b = ref_other.p->field_64;
    if (config->field_4e | config->field_4d) {
        return 0;
    }
    if ((u8)b == 0) {
        b = 1;
    }
    slot = a & 0x1f;
    object->field_be = slot;
    object->field_bf = 0xff;
    func_80155d4c(table_80181510[slot], (s8)object->side);
    ref_other.p->health = ref_other.p->health - (u8)b;
    if ((s16)ref_other.p->health < 0 && ((a >> 8) & 0x80)) {
        ref_other.p->health = 0;
    }
    return (s16)ref_other.p->health < 0;
}
