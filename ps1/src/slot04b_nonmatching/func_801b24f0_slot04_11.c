/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes (it is 580 bytes, the
 * original 592). The exact owner of the bytes in the PS1 build stays the
 * raw bytes of the module image; the build does not use this file. The
 * differential test next to it (func_801b24f0_slot04_11.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): a character's step
 * handler of a move that can end in one of three ways. It first moves the
 * object by its speed words (func_801b5764). While the current animation
 * step is not the last one (bit 15 of field_3a clear) it only calls
 * func_80130efc. Otherwise it picks a case. For an object with field_cd
 * set: a table test (func_8014a170) that answers yes gives the "stop"
 * case; otherwise bit 0 of the random generator (func_80151184) gives the
 * "approach" case when clear and the "walk" case when set. For one with
 * field_cd clear the input field_130 gives "approach" when bit 0x1000 is
 * set, else "stop" when bit 0x2000 is set, else "walk".
 *   Approach: field_07 advances; the other object's (field_40) position
 *   word at field_10 minus this one's, over 16, becomes field_4c, a value
 *   from field_70 and field_14 becomes field_50, field_54 and field_58 are
 *   cleared, sequence 0x2c starts.
 *   Stop: field_07 = 7; field_50 = 0x10000, field_54 = 0, field_58 =
 *   -0x6000, field_4c = 0x18000 (negated when field_0b is clear); sequence
 *   0x34 starts.
 *   Walk: field_07 = 6, field_48 = 0. A side is chosen: for field_cd set
 *   (after func_801b5914) the right side when field_0b is clear, else bit
 *   0x8000 of field_130. Four words of the table at row field_12a (two
 *   words to a row, twelve more for the right side) become field_4c
 *   (negated when field_0b is clear), field_54, field_50, field_58; when
 *   field_49 is set field_4c and field_50 double. Sequence 0x26 (0x27 for
 *   the right side) plus field_12a starts.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_0b, field_10 (word), field_14 (word), field_3a,
 *     field_40 (other object), field_49, field_70, field_cd, field_cf
 *     (cd set), field_12a, field_130 (cd clear), and what func_801b5764
 *     reads (field_4c, field_50, field_54, field_58); the other object's
 *     field_10 and, through func_801b5914, its table entry
 *     (data_801c7844_slot04_11 at field_0e); the table words
 *     data_801c711c_slot04_11 at field_cf and data_801c7450_slot04_11 at the
 *     rows named above; the seed data_80190126.
 *   Writes: field_07, field_48, field_10, field_14, field_4c, field_50,
 *     field_54, field_58, ref_other (approach case), the seed.
 *   Callees replaced by recorders: func_801307e0 (2 arguments) and
 *     func_80130efc (1); they reach the sequence tables and the library,
 *     their results are unused.
 *   Callees run as original code: func_801b5764, func_801b5914,
 *     func_8014a170, func_80151184.
 *   Watched at every recorded call (the test copies them into the log): the
 *     object's whole block, ref_other, and the word that holds the seed.
 *   Aliasing: the object and the other object are distinct blocks.
 *   Excluded inputs: none. field_12a may be any byte; the rows then lie
 *     past the table in mapped RAM, which both runs read alike.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Functions of this image that the shared headers do not declare, as the image's units declare them. */
int func_801b5764_slot04_11(Object *obj);
Object *func_801b5914_slot04_11(Object *obj);

extern u32 data_801c711c_slot04_11[];
extern s32 data_801c7450_slot04_11[];

enum { MOVE_WALK, MOVE_APPROACH, MOVE_STOP };

void func_801b24f0_slot04_11(Object *obj) {
    int input;
    int move;
    int right;
    int row;
    int seq;
    int dist;
    int speed_x;
    int speed_y;
    int speed_z;
    int speed_w;

    func_801b5764_slot04_11(obj);
    if ((obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
        return;
    }
    input = 0;
    if (obj->field_cd != 0) {
        if (func_8014a170(obj, data_801c711c_slot04_11) != 0) {
            move = MOVE_STOP;
        } else if ((func_80151184() & 1) == 0) {
            move = MOVE_APPROACH;
        } else {
            move = MOVE_WALK;
        }
    } else {
        input = obj->field_130;
        if (input & 0x1000) {
            move = MOVE_APPROACH;
        } else if (input & 0x2000) {
            move = MOVE_STOP;
        } else {
            move = MOVE_WALK;
        }
    }
    if (move == MOVE_APPROACH) {
        obj->field_07++;
        ref_other.p = obj->other;
        dist = *(s32 *)&ref_other.p->field_10 - *(s32 *)&obj->field_10;
        obj->field_54 = 0;
        obj->field_58 = 0;
        obj->field_4c = dist >> 4;
        dist = ((obj->field_70 - 0x8a) << 16) - *(s32 *)&obj->field_14;
        dist = (dist >> 4) - 0x4000;
        obj->field_50 = -dist;
        func_801307e0(obj, 0x2c);
        return;
    }
    if (move == MOVE_STOP) {
        speed_x = 0x18000;
        obj->field_07 = 7;
        if (obj->field_0b == 0) {
            speed_x = -0x18000;
        }
        obj->field_50 = 0x10000;
        obj->field_4c = speed_x;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
        func_801307e0(obj, 0x34);
        return;
    }
    obj->field_07 = 6;
    obj->field_48 = 0;
    if (obj->field_cd != 0) {
        func_801b5914_slot04_11(obj);
        right = obj->field_0b == 0;
    } else {
        right = (input & 0x8000) != 0;
    }
    row = obj->field_12a * 2;
    seq = 0x26;
    if (right) {
        seq = 0x27;
        row += 12;
    }
    speed_x = data_801c7450_slot04_11[row];
    speed_y = data_801c7450_slot04_11[row + 1];
    speed_z = data_801c7450_slot04_11[row + 2];
    speed_w = data_801c7450_slot04_11[row + 3];
    if (obj->field_0b == 0) {
        speed_x = -speed_x;
    }
    if (obj->field_49 != 0) {
        speed_x <<= 1;
        speed_z <<= 1;
    }
    obj->field_4c = speed_x;
    obj->field_54 = speed_y;
    obj->field_50 = speed_z;
    obj->field_58 = speed_w;
    func_801307e0(obj, seq + obj->field_12a);
}
