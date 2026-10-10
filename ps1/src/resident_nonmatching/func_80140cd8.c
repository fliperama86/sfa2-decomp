/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register use and frame
 * size (the original recomputes `a & 0x1f` where this build keeps the first
 * copy; read from the original's listing, not tested). The exact owner of
 * the bytes in the PS1 build stays the raw bytes of the resident
 * executable; the build does not use this file. The differential test next
 * to it (func_80140cd8.py, run by difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): the sibling of
 * func_80140b5c. It makes ref_other point at the object's partner
 * (object->other) and computes a byte in the partner's field_64 from the
 * same tables, in the same way, except that a value of 0x80 or more is set
 * to 0x7f (func_80140b5c clears it when bit 7 is set), there is no early
 * return for a == -1, and the sum of the random number and b << 5 is
 * added, not or-ed. After the handler call (func_80140ec0) it returns 0 at
 * once when either of two configuration bytes (fields 4d, 4e) is set.
 * Otherwise it
 * takes the byte (at least 1), records the low five bits of a in
 * object->field_be and 0xff in field_bf, calls func_80155d4c with a table
 * byte selected by those five bits and the object's side, subtracts the
 * byte from the partner's 16-bit field_5c, clears field_5c when it went
 * negative and bit 15 of a is set, and returns 1 when field_5c is negative,
 * else 0.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = a (32 bits, bits 0 to 4 and bit 15 used),
 *     a2 = b (32 bits). Result: v0 = 0 or 1.
 *   Reads: object->other, field_cf, kind, side; table_801777d4,
 *     table_80177bd4 (16-bit index, negative values read before the table,
 *     in the resident image's own data), table_80179834, table_8017984c,
 *     table_80181510; the partner's field_64 and field_5c; game_state.config
 *     fields 4d and 4e; game_state.field_12 (handler index); the random
 *     state of func_80151184 (the seed halfword data_80190126).
 *   Writes: ref_other, game_state.cursor, the partner's field_64 and field_5c,
 *     object->field_be and field_bf, the random state.
 *   func_80151184 and func_80140ec0 run as the original code. The handler
 *     it calls through table_8017ac34[game_state.field_12] is a recorder with
 *     no arguments (entries 0 to 3 of the table point at such blocks and
 *     field_12 stays below 4); func_80155d4c is a recorder with two
 *     arguments, no pointee, result 0. Their real code is outside this test.
 *     Watched at every recorded call: the whole object, the whole partner,
 *     ref_other and game_state.cursor.
 *   Aliasing: the object, the partner and the configuration block are
 *     distinct blocks.
 *   Exclusions: none. All instruction slots are reachable (inferred).
 * The tree declares func_80155d4c with int arguments; the side is cast to s8
 * at the call, as the original passes it sign-extended (read from the
 * original's listing, not tested).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"


int func_80140cd8(Object *object, int a, int b) {
    Object *target;
    int idx;
    int slot;
    u8 value;

    ref_other.p = object->other;
    target = ref_other.p;
    target->field_64 = table_801777d4[object->field_cf + ((a & 0x1f) << 5)];
    idx = (s16)((func_80151184() & 0x1f) + (b << 5));
    target->field_64 += 0xf0 + table_80177bd4[idx];
    game_state.cursor = (u32 *)table_8017984c[table_80179834[object->kind]];
    value = target->field_64;
    if (value >= 0x80) {
        value = 0x7f;
    }
    target->field_64 = value + ((u8 *)game_state.cursor)[value];
    func_80140ec0();
    if ((game_state.config->field_4e | game_state.config->field_4d) != 0) {
        return 0;
    }
    value = target->field_64;
    if (value == 0) {
        value = 1;
    }
    slot = a & 0x1f;
    object->field_be = slot;
    object->field_bf = 0xff;
    func_80155d4c(table_80181510[slot], (s8)object->side);
    target->field_5c -= value;
    if ((s16)target->field_5c < 0 && (a & 0x8000)) {
        target->field_5c = 0;
    }
    return (s16)target->field_5c < 0;
}
