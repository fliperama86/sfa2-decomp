/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



u8 func_8012faf8(Object *object) {
    s16 d;
    u8 r;

    ref_other.p = object->other;
    if (object->field_45 != 0 && ref_other.p->field_4b == 0 && ref_other.p->field_7e == 0 &&
        ref_other.p->field_45 == 0) {
        return 0;
    }
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1 && (object->field_1a1 & 0x80) == 0 &&
        object->field_6a == 0 && object->field_1a2 != 0) {
        object->field_1a0 = object->field_1a0 + 1;
        return 1;
    }
    if (ref_other.p->field_7e == 0 && ref_other.p->field_225 == 0) {
        if (ref_other.p->field_159 == 0) {
            return 0;
        }
        r = func_8012fd80(object, ref_other.p);
        if (r != 0 && (r != 1 || (d = ref_other.p->pos_x - object->pos_x, (d < 0 ? d = -d : d), (s16)(d + 0xa0) < 0x141))) {
        } else {
        zero:
            return 0;
        }
    }
    if (object->field_150 & 0x2000) {
        if (object->field_d8 != 0) {
            object->field_24e = 1;
            object->field_251 = 1;
        }
    } else if (object->field_d8 == 0) {
        if (game_state.field_30 != 0 && game_state.mode != object->side + 1 &&
            (object->field_1a1 & 0x80) == 0 && object->field_6a == 0 && object->field_1a2 != 0) {
            object->field_1a0 = object->field_1a0 + 1;
            return 1;
        }
        if (((GameState *)game_state.config)->field_bb == 0) {
            goto zero;
        }
        return (((GameState *)game_state.config)->field_15b & ((GameState *)game_state.config)->field_224) != 0;
    } else {
        object->field_24e = 1;
        object->field_251 = 1;
        return 1;
    }
    return 1;
}
