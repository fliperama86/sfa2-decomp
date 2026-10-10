/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is a different length and orders some loads differently. The
 * PS1 build keeps the raw bytes of the module image and does not use this
 * file. The differential test next to it (difftest.py, with
 * func_801b3564_slot04_06.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a per-frame update of a
 * character state, chosen by the byte field_3a.
 *   field_3a == 2: if pos_x is not in 0x201..0x300, field_0b becomes 1 when
 *     pos_x < 0x280 and 0 otherwise; then func_80130efc.
 *   field_3a == 0: func_80130efc.
 *   Otherwise, with field_4c >= 0: func_801b4818_slot04_06 (the move) runs.
 *     If field_cd is not 0, func_80141248 runs and a non-zero low byte of
 *     func_801410c8 ends the state (below). Then, when bit 0 of
 *     game_state.field_1d is clear and either field_1c5 is below
 *     (field_12a >> 1) + 3 or has its bit 7 set, a non-zero low byte of
 *     func_801b5ed8_slot04_06 knocks the object ref_other.p back
 *     (func_801b4874_slot04_06 on a halfword d = (random & 0xf) + 0x5c,
 *     ref_other.p->pos_x += d, ref_other.p->pos_y -= random & 0xf).
 *     Then, when bits 0 and 1 of game_state.field_1d are clear, a hit test
 *     decides whether the same knock-back (with d = 0x60) is done: with
 *     field_1c5 == 1 the test is the low byte of func_801468f4, otherwise
 *     func_80148e84 when a random & 0xf is 0 and func_80148ea8 when not.
 *     The function ends with func_80130efc.
 *   Otherwise (field_4c negative): field_1c5 is decremented; when it is 0
 *     the state ends. When not: func_80140cd8 (object, -0xfe, 0);
 *     func_801204f4 (object, side, 8); func_80120554 (other, other's side,
 *     0x34e or 0x34f, by the sign of other's halfword field_5c; other =
 *     field_40); field_4c = 0xa0000, field_54 = -0xe000,
 *     game_state.field_63 = 8; then sequence data_801c547c_slot04_06
 *     [field_1c5] (halfword table) is started with func_801307e0.
 *   End of the state: field_07 + 1 and func_801307e0 (object, 0x2e).
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_3a, pos_x, field_4c, field_cd, field_12a, field_1c5,
 *     side, field_0b (by the knock-back helper), field_40 and, of that
 *     object, field_5c and side; game_state.field_1d; the pointer ref_other.p and that object's pos_x and pos_y;
 *     the halfword table at data_801c547c_slot04_06 (the setup fills 256).
 *   Writes: field_0b, field_4c, field_54, field_07, field_1c5, pos_x and
 *     pos_y of the ref_other.p object, game_state.field_63, plus what the
 *     callees write (the move writes the words at field_10 and
 *     field_4c).
 *   Callees: func_801b4818_slot04_06 (the move, 1 argument) and
 *     func_801b4874_slot04_06 (2 arguments, a halfword at a pointer) run as
 *     the original code in both runs; they touch only the object and the
 *     halfword. Replaced by recorders, in the original and in this C alike:
 *     func_80130efc (1), func_801307e0 (2), func_80140cd8 (3), func_801204f4
 *     (3), func_80120554 (3), func_80141248 (1), func_801410c8 (1),
 *     func_801b5ed8_slot04_06 (1), func_801468f4 (1), func_80148e84 (1),
 *     func_80148ea8 (1), func_80151184 (0, a different random value at
 *     each call). The results of the five tested callees are random per
 *     case, zero in half of the cases.
 *   Aliasing: the object, field_40's object, and the ref_other.p object are
 *     distinct blocks of 0x394 bytes.
 *   Watched by the recorders: the whole object, the field_40 object, the
 *     ref_other.p object and game_state (0x364 bytes) at every call, so the
 *     order of this function's stores against the calls is tested. The
 *     halfword that the knock-back passes by pointer is a local of the
 *     function, not in a watched block; its helper runs as the original
 *     code, so its effect shows in pos_x of the ref_other.p object.
 *   Inputs excluded: none.
 *   Slots not reached: four, at the offsets 0x8c, 0x90, 0x94 and 0x98 from
 *     the start (the original's 0x801b35f0 to 0x801b35fc: the stores of
 *     game_state.field_63 = 0x18 and the call of func_80146960). They are
 *     in the arm where field_3a == 2 is tested a second time after it was
 *     found different, so no input reaches them. They are not in this C.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations, not original ones. */
void func_80141248(Object *object);
int func_801410c8(Object *object);
int func_801468f4(Object *object);
void func_801b4818_slot04_06(Object *obj);
void func_801b4874_slot04_06(Object *obj, u16 *p);
u8 func_801b5ed8_slot04_06(Object *obj);
extern u16 data_801c547c_slot04_06[];

/* Pushes the object that ref_other points at away: x by d as the helper
   signs it, y up by a random 0..15 (inferred role). */
static void knock_back(Object *obj, u16 d) {
    func_801b4874_slot04_06(obj, &d);
    ref_other.p->pos_x = ref_other.p->pos_x + d;
    ref_other.p->pos_y = ref_other.p->pos_y - (func_80151184() & 0xf);
}

void func_801b3564_slot04_06(Object *obj) {
    u8 hit;

    if ((u8)obj->field_3a == 2) {
        if (obj->pos_x <= 0x200 || obj->pos_x >= 0x301) {
            obj->field_0b = obj->pos_x < 0x280;
        }
        func_80130efc(obj);
        return;
    }
    if ((u8)obj->field_3a == 0) {
        func_80130efc(obj);
        return;
    }
    if (obj->field_4c < 0) {
        ((Slot04aObj *)obj)->field_1c5--;
        if (((Slot04aObj *)obj)->field_1c5 != 0) {
            func_80140cd8(obj, -0xfe, 0);
            func_801204f4(obj, obj->side, 8);
            if ((s16)obj->other->field_5c >= 0) {
                func_80120554(obj->other, obj->other->side, 0x34e);
            } else {
                func_80120554(obj->other, obj->other->side, 0x34f);
            }
            obj->field_4c = 0xa0000;
            obj->field_54 = -0xe000;
            game_state.field_63 = 8;
            func_801307e0(obj, data_801c547c_slot04_06[((Slot04aObj *)obj)->field_1c5]);
            return;
        }
        obj->field_07++;
        func_801307e0(obj, 0x2e);
        return;
    }
    func_801b4818_slot04_06(obj);
    if (obj->field_cd != 0) {
        func_80141248(obj);
        if ((u8)func_801410c8(obj) != 0) {
            obj->field_07++;
            func_801307e0(obj, 0x2e);
            return;
        }
    }
    if ((game_state.field_1d & 1) == 0) {
        if (((Slot04aObj *)obj)->field_1c5 < (u32)(obj->field_12a >> 1) + 3 || (((Slot04aObj *)obj)->field_1c5 & 0x80) != 0) {
            if (func_801b5ed8_slot04_06(obj) != 0) {
                knock_back(obj, (func_80151184() & 0xf) + 0x5c);
            }
        }
    }
    if ((game_state.field_1d & 3) == 0) {
        if (((Slot04aObj *)obj)->field_1c5 == 1) {
            hit = func_801468f4(obj);
        } else if ((func_80151184() & 0xf) != 0) {
            hit = func_80148ea8(obj);
        } else {
            hit = func_80148e84(obj);
        }
        if (hit != 0) {
            knock_back(obj, 0x60);
        }
    }
    func_80130efc(obj);
}
