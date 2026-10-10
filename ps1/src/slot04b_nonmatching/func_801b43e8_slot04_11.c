/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes. The exact owner of the
 * bytes in the PS1 build stays the raw bytes of the module image; the build
 * does not use this file. The differential test next to it
 * (func_801b43e8_slot04_11.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a character's step handler
 * of a one-key move. It first lets the input collector (func_801b4970) run.
 * While the current animation step is not the last one (bit 15 of field_3a
 * clear) it only calls func_80130efc. Otherwise it advances field_07 by 3
 * and forms a direction code. For an object with field_cd clear the code
 * is the halfword field_1c0 (the collector's result). For one with
 * field_cd set the code comes from a table test (func_8014a170): when it
 * answers yes, from a random pick (func_801b49e8); when no, from the
 * distance of the other object's field_12 to this one's (8 when 0x100 or
 * more, 0x20 from 0x80, else 0x40), and ref_other is set to field_40. The
 * code goes through func_801b49b8 (which mirrors it when field_0b is set).
 * A variant k is then 0 when bit 3 (value 8) of the code is set, 2 when 0x40 is
 * set, else 1. The move reads four words of a table at row field_12a, entry
 * k (eight words to a row): speed words for field_4c, field_54, field_50,
 * field_58, with the first two negated when field_0b is clear. It counts
 * field_12e up, clears field_1c0, and starts the sequence k plus the table
 * word at row field_12a, offset field_12e (old value) plus 12.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_07, field_0b, field_0e (cd set), field_12
 *     (s16), field_3a, field_40 (cd set), field_cd, field_cf (cd set),
 *     field_12a, field_12e, field_134 and field_1c0 (halfword, through
 *     func_801b4970); the other object's field_12; the module tables
 *     data_801c719c_slot04_11 at field_cf, data_801c7798_slot04_11 at the
 *     code divided by 8 (when field_0b is set), data_801c77a8_slot04_11,
 *     data_801c7844_slot04_11 at field_0e, data_801c7648_slot04_11 at the
 *     entries named above; the seed data_80190126 (func_8014a170 and
 *     func_801b49e8 run func_80151184).
 *   Writes: field_07, field_12e, field_1c0, field_4c, field_50, field_54,
 *     field_58, ref_other (when field_cd is set), the seed data_80190126.
 *   Callees replaced by recorders: func_801307e0 (2 arguments) and
 *     func_80130efc (1); they reach the sequence tables and the library,
 *     their results are unused.
 *   Callees run as original code: func_801b4970, func_801b49b8,
 *     func_801b49e8, func_801b5914, func_8014a170, func_80151184.
 *   Watched at every recorded call (the test copies them into the log): the
 *     object's whole block, ref_other, and the word that holds the seed.
 *   Aliasing: the object and the other object are distinct blocks.
 *   Excluded inputs: none. The code is read at index (code >> 3) of a byte
 *     table, and the table words at rows of field_12a up to 255 reach past
 *     the tables into mapped RAM; both runs read the same bytes.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Functions of this image that the shared headers do not declare, as the image's units declare them. */
void func_801b4970_slot04_11(Object *obj);
u8 func_801b49b8_slot04_11(Object *obj, int a);
u8 func_801b49e8_slot04_11(Object *obj);
Object *func_801b5914_slot04_11(Object *obj);

extern u32 data_801c719c_slot04_11[];
extern s32 data_801c7648_slot04_11[];

void func_801b43e8_slot04_11(Object *obj) {
    int code;
    int variant;
    int row;
    int dx;
    int seq;
    int speed_x;
    int speed_y;

    func_801b4970_slot04_11(obj);
    if ((obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
        return;
    }
    code = *(u16 *)&((Slot04bObj *)obj)->field_1c0;
    obj->field_07 += 3;
    if (obj->field_cd != 0) {
        if (func_8014a170(obj, data_801c719c_slot04_11) != 0) {
            code = func_801b49e8_slot04_11(obj);
        } else {
            func_801b5914_slot04_11(obj);
            ref_other.p = obj->other;
            dx = ref_other.p->pos_x - obj->pos_x;
            code = 8;
            if (dx < 0x100) {
                code = 0x20;
                if (dx < 0x80) {
                    code = 0x40;
                }
            }
        }
    }
    code = func_801b49b8_slot04_11(obj, code);
    variant = 0;
    if ((code & 8) == 0) {
        variant = 1;
        if (code & 0x40) {
            variant = 2;
        }
    }
    row = obj->field_12a * 8;
    seq = row + obj->field_12e + 12;
    obj->field_12e++;
    speed_x = data_801c7648_slot04_11[row + variant * 4];
    speed_y = data_801c7648_slot04_11[row + variant * 4 + 1];
    obj->field_50 = data_801c7648_slot04_11[row + variant * 4 + 2];
    obj->field_58 = data_801c7648_slot04_11[row + variant * 4 + 3];
    if (obj->field_0b == 0) {
        speed_x = -speed_x;
        speed_y = -speed_y;
    }
    *(u16 *)&((Slot04bObj *)obj)->field_1c0 = 0;
    obj->field_4c = speed_x;
    obj->field_54 = speed_y;
    func_801307e0(obj, variant + data_801c7648_slot04_11[seq]);
}
