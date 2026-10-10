/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes (it is 504 bytes, the
 * original 528). The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (func_801b27a0_slot04_17.py) compares
 * the behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): a character's action
 * handler. While the current animation step is not the last one (bit 15
 * of field_3a clear) it only calls func_80130efc. Otherwise it first tests
 * for a "stop" input: for an object with field_cd set a table test
 * (func_8014a170), for one with field_cd clear bit 0x1000 of field_130.
 * A stop ends the move: field_07 = 7, fixed velocity words, a quarter of
 * the old field_4c, and sequence 0x34. Without a stop it advances field_07
 * and calls func_801204f4. A direction input is then formed: from
 * field_130 when field_cd is clear, and when field_cd is set from the sign
 * of the other object's field_12 minus this one's (inside a dead zone of
 * about 16 units, none; field_0b flips the sides). A direction bit (0x8000
 * or 0x2000) selects the walking speed words, the sign of field_4c taken
 * from a two-word table by field_0b, and flips field_0b when bit 0x2000 is
 * set; no direction selects the standing words. field_4c and field_50
 * double when field_49 is set. Then it starts sequence 0x2e or 0x2f plus
 * field_12a.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_07, field_0b, field_12 (s16), field_3a,
 *     field_40 (other object), field_49, field_4c, field_a6, field_cd,
 *     field_cf (when field_cd is set), field_12a, field_130; the other
 *     object's field_12; the word of data_801ce050_slot04_17 at index
 *     field_cf and the word of data_801ce3e4_slot04_17 at index field_0b
 *     (both fixed tables of the module); the random seed word
 *     data_80190126 (through func_8014a170, which runs as original code).
 *   Writes: field_07, field_0b, field_48, field_4c, field_50, field_54,
 *     field_58, ref_other (set to field_40 when field_cd is set), and the
 *     seed data_80190126.
 *   Callees replaced by recorders (they reach sequence tables and the
 *     library's sound): func_801204f4 (3 arguments), func_801307e0 (2),
 *     func_80130efc (1); they write nothing the test compares except
 *     their log, and their results are not used.
 *   Callee run as original: func_8014a170 (game code; reads field_cf and
 *     the seed).
 *   Watched at every recorded call: the whole object (0x400 bytes), the whole
 *     other object, ref_other. No recorded callee
 *     gets a pointer to memory filled for the call.
 *   Aliasing: the object and the other object are distinct blocks.
 *   Excluded inputs: none. field_0b and field_cf may be any byte; the
 *     tables are read at those indices (4 bytes each, inside mapped RAM).
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801ce050_slot04_17[];
extern s32 data_801ce3e4_slot04_17[];

void func_801b27a0_slot04_17(Object *obj) {
    int input;
    int stop;
    int speed_4c;
    int speed_50;
    int speed_58;
    int walk;
    int dx;
    s16 dir;

    if ((obj->field_3a << 16) >= 0) {
        func_80130efc(obj);
        return;
    }
    input = obj->field_130;
    if (obj->field_cd != 0) {
        stop = func_8014a170(obj, data_801ce050_slot04_17) != 0;
    } else {
        stop = (input & 0x1000) != 0;
    }
    if (stop) {
        speed_4c = obj->field_4c >> 2;
        obj->field_07 = 7;
        obj->field_50 = 0x10000;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
        obj->field_4c = speed_4c;
        func_801307e0(obj, 0x34);
        return;
    }
    obj->field_07++;
    func_801204f4(obj, obj->side, 6);
    if (obj->field_cd != 0) {
        ref_other.p = obj->other;
        dx = ref_other.p->pos_x - obj->pos_x;
        input = -0x8000;
        if (dx >= 0) {
            input = 0x2000;
        }
        if (obj->field_0b != 0) {
            input ^= 0xa000;
        }
        if (dx + 0x10 < 0x21) {
            input = 0;
        }
    }
    walk = 1;
    speed_4c = 0;
    speed_50 = -0x8000;
    speed_58 = -0x8000;
    dir = input & -0x6000;
    if (dir != 0) {
        walk = 0;
        speed_4c = 0x28000;
        speed_58 = -0x6000;
        if ((dir & data_801ce3e4_slot04_17[obj->field_0b]) == 0) {
            speed_4c = -0x28000;
        }
        if (dir & 0x2000) {
            obj->field_0b ^= 1;
        }
    }
    obj->field_48 = walk;
    if (obj->field_49 != 0) {
        speed_4c <<= 1;
        speed_50 <<= 1;
    }
    obj->field_4c = speed_4c;
    obj->field_54 = 0;
    obj->field_50 = speed_50;
    obj->field_58 = speed_58;
    func_801307e0(obj, walk + 0x2e + obj->field_12a);
}
