/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in which register holds the
 * or-ed value (read from the original's listing, not tested). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the resident
 * executable; the build does not use this file. The differential test next
 * to it (func_80140b5c.py, run by difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): computes a byte value in
 * field_64 of the object that ref_other points at, and returns it. With a
 * == -1 it returns 0 at once. Otherwise it starts from a table byte chosen
 * by the object's field_cf and the low five bits of `a` (32 per step),
 * adds 0xf0 and a second table byte chosen by a random number (low five
 * bits) combined with b << 5, clears the result when bit 7 is set, selects
 * a row of table_8017984c through table_80179834[object->kind] (the row's
 * address is stored in game_state.cursor), adds that row's entry for the
 * current value, calls func_80140ec0 (which calls through table_8017ac34, a
 * handler that may change field_64; read from the original's listing, not
 * tested), and returns field_64 halved when field_15b is 0, but at least 1.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = a (16 bits, the upper half of the register
 *     is ignored), a2 = b (32 bits). Result: v0 in 0 to 255.
 *   Reads: object->field_cf and ->kind; table_801777d4, table_80177bd4
 *     (index the 16-bit value (rand & 0x1f) | (b << 5), so negative values
 *     read before the table, in the resident image's own data),
 *     table_80179834, table_8017984c (rows of 128 bytes), ref_other.p and
 *     its field_64 and field_15b, game_state.field_12 (the handler index;
 *     read by func_80140ec0, not by this C).
 *   Writes: ref_other.p->field_64, game_state.cursor, and the random state
 *     func_80151184 keeps, the seed halfword data_80190126 (read from the
 *     original's listing, not tested). func_80151184 and func_80140ec0 run as
 *     the original code, as does the code before the handler.
 *   The handler called through table_8017ac34[game_state.field_12] is a
 *     recorder with no arguments that returns at once; the setup points the
 *     entries 0 to 3 at such blocks and keeps game_state.field_12 below 4.
 *     Its real code is not part of this test. The log watches, at every
 *     call, the whole target of ref_other and game_state.cursor.
 *   Aliasing: the object and the target of ref_other are distinct blocks.
 *   Not reached by any input: the original clamps the value to 0x7f when it
 *     is 0x80 or more (slot at offset 0x100, one instruction; read from the
 *     original's listing, not tested). Bit 7 was
 *     cleared just before, so the value is below 0x80; this C omits the
 *     clamp.
 *   Exclusions: none otherwise.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140b5c(Object *object, s16 a, int b) {
    Object *target = ref_other.p;
    int idx;
    u8 value;

    if (a == -1) {
        return 0;
    }
    target->field_64 = table_801777d4[object->field_cf + ((a & 0x1f) << 5)];
    idx = (s16)((func_80151184() & 0x1f) | (b << 5));
    target->field_64 += 0xf0 + table_80177bd4[idx];
    if (target->field_64 & 0x80) {
        target->field_64 = 0;
    }
    game_state.cursor = (u32 *)table_8017984c[table_80179834[object->kind]];
    value = target->field_64;
    target->field_64 = value + ((u8 *)game_state.cursor)[value];
    func_80140ec0();
    value = target->field_64;
    if (target->field_15b == 0) {
        value >>= 1;
    }
    if (value == 0) {
        value = 1;
    }
    return value;
}
