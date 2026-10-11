/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"


/*
 * What it does (inferred): tries to start an action for the object. Unless
 * the object's bytes 4 to 7 hold exactly 1, 1, 2, 0 it first checks that the
 * object is idle (field_7e and field_45 are 0, bytes 4 and 5 are 1 and 0,
 * byte 6 is not 7, 8 or 9); otherwise it returns 0. It clears field_69 and
 * field_6a, clears field_19f and field_1a2 when game_state.mode differs from
 * the object's side plus 1, returns 0 when a configuration byte or the
 * partner test is set, and otherwise calls func_80142c04 and returns 1.
 */
u8 func_80141cec(Object *object) {
    short checked = 0;
    u32 partial;
    u32 idle;
    u32 stand_in;
    u32 w = *(u32 *)&object->state_0;

    if (w != 0x20101) {
        checked = 1;
        partial = (u16)w;

        partial = object->field_7e | object->field_45 | (partial != 1);
        idle = partial | (object->state_2 == 7);
        partial = idle | (object->state_2 == 8);
        /* Stand-in, not recovered source: this copy local reproduces the original's choice of registers. Without it this function differs from the original in 2 instruction slots. */
        stand_in = partial;
        if (object->state_2 == 9) return 0;
        if (stand_in != 0) return 0;
    }
    object->field_69 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0) {
        if (game_state.mode != (object->side + 1)) {
            object->field_19f = 0;
            object->field_1a2 = 0;
        }
    }
    if ((game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) != 0) {
        return 0;
    }
    if ((func_8012f56c(object) & 0xff) != 0) {
        return 0;
    }
    if (!checked) {
        object->field_bd = 2;
        if (object->field_cd == 0) {
            func_80155d4c(0xd, (s8)object->side);
        }
    }
    object->field_157 = 0;
    object->field_4b = object->field_25c;
    func_80142c04(object);
    return 1;
}
