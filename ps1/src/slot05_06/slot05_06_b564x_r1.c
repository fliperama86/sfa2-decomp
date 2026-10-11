/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80141248(Object *object);
int func_801468f4(Object *object);
void func_801cc814_slot05_06(Object *obj);
void func_801cc870_slot05_06(Object *obj, u16 *p);
int func_801cded4_slot05_06(Object *obj);
extern u16 data_801dd478_slot05_06[];

/*
 * What it does (inferred, not an original name): a per-frame update of a
 * character state chosen by field_3a. State 2 sets field_0b from pos_x;
 * state 0 does nothing; otherwise, with field_4c >= 0, it runs the move,
 * may knock back ref_other.p, then func_80130efc. With field_4c negative
 * it counts field_1c5 down and either ends the state (field_07 + 1,
 * sequence 0x2e) or starts the sequence from data_801dd478_slot05_06.
 * The twin of func_801b3564_slot04_06 on the second side (same code, a callee that returns an int).
 * Casts to Slot04aObj: field_1c5 is a member of the module's record type
 * only; a local pointer of that type instead of the casts differs from
 * the original in 49 instruction lines.
 */
void func_801cb564_slot05_06(Object *obj) {
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With game_state written at its use (the store of field_63 at the end) this function differs from the original in 32 instruction lines. */
    GameState *state = &game_state;
    /* The local holds a constant: the original keeps this value in a register of its own (a second one, copied from the first). The name is inferred. */
    /* With game_state written at its two uses (the tests of field_1d) this function differs from the original in 33 instruction lines; with game_state written at all uses, in 45. */
    GameState *state_late = &game_state;
    u8 kind = obj->field_3a;
    u8 hit;
    u16 d;
    if (kind == 2) {
        if (obj->pos_x > 0x200) {
            if (obj->pos_x < 0x301) {
                func_80130efc(obj);
                return;
            }
        }
        obj->field_0b = 0;
        if (obj->pos_x < 0x280) {
            obj->field_0b = 1;
        }
        func_80130efc(obj);
        return;
    }
    if (kind == 0) {
        func_80130efc(obj);
        return;
    }
    /* Exception, not recovered source: this test can never be true (kind is not 2 here, the first test returned), but the original has the branch (bne v0,v1 with v1 still 2, then the store of 0x18 and the call). Without it this function differs from the original in 33 instruction lines. */
    if (kind == 2) {
        game_state.field_63 = 0x18;
        func_80146960(obj);
    }
    if (obj->field_4c >= 0) {
        func_801cc814_slot05_06(obj);
        if (obj->field_cd != 0) {
            func_80141248(obj);
            /* The result is masked to a byte; without the cast this function differs from the original in 25 instruction lines. */
            if ((u8)func_801410c8(obj) != 0) {
                obj->field_07++;
                func_801307e0(obj, 0x2e);
                return;
            }
        }
        if ((state_late->field_1d & 1) == 0) {
            /* The unsigned cast changes no value (a byte plus 3); without it this function differs from the original in 1 instruction line. */
            if (((Slot04aObj *)obj)->field_1c5 < (u32)(obj->field_12a >> 1) + 3 || (((Slot04aObj *)obj)->field_1c5 & 0x80) != 0) {
                if (func_801cded4_slot05_06(obj) != 0) {
                    d = (func_80151184() & 0xf) + 0x5c;
                    func_801cc870_slot05_06(obj, &d);
                    ref_other.p->pos_x += d;
                    d = func_80151184();
                    d &= 0xf;
                    ref_other.p->pos_y -= d;
                }
            }
        }
        if ((state_late->field_1d & 3) == 0) {
            /* Exception, not recovered source: the four labels (b2, call, test, alt) and the gotos to them. The original shares the knock-back block (b2) and the final call between three paths; written with nested ifs and a flag the function differs from the original in 30 instruction lines. */
            if (((Slot04aObj *)obj)->field_1c5 == 1) {
                /* The result is read as a signed byte; without the cast this function differs from the original in 20 instruction lines. */
                if ((s8)func_801468f4(obj) != 0) {
                    goto b2;
                }
                goto call;
            }
            d = func_80151184();
            if ((d &= 0xf) != 0) {
                goto alt;
            }
            hit = func_80148e84(obj);
test:
            if (hit != 0) {
                goto b2;
            }
            goto call;
alt:
            hit = func_80148ea8(obj);
            goto test;
b2:
            d = 0x60;
            func_801cc870_slot05_06(obj, &d);
            ref_other.p->pos_x += d;
            d = func_80151184();
            d &= 0xf;
            ref_other.p->pos_y -= d;
        }
call:
        func_80130efc(obj);
    } else {
        ((Slot04aObj *)obj)->field_1c5--;
        if (((Slot04aObj *)obj)->field_1c5 != 0) {
            func_80140cd8(obj, -0xfe, 0);
            func_801204f4(obj, obj->side, 8);
            /* Without the signed cast the compiler warns and this function differs from the original in 45 instruction lines. */
            if ((s16)obj->other->field_5c >= 0) {
                func_80120554(obj->other, obj->other->side, 0x34e);
            } else {
                func_80120554(obj->other, obj->other->side, 0x34f);
            }
            obj->field_4c = 0xa0000;
            obj->field_54 = -0xe000;
            state->field_63 = 8;
            d = data_801dd478_slot05_06[((Slot04aObj *)obj)->field_1c5];
            func_801307e0(obj, d);
        } else {
            obj->field_07++;
            func_801307e0(obj, 0x2e);
        }
    }
}
