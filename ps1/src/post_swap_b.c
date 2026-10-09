/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int func_8013d0fc(Object *object);

void func_80132370(Object *object) {
    if (object->field_cd != 0 && object->field_cf >= 0x1d &&
        game_state.field_33 == 0 &&
        (object->field_67 != 0 || ((s16)object->field_3a & 0x8000)) &&
        object->field_14c == 0 &&
        object->pos_y < (s16)(object->field_70 - 0x30)) {
        object->field_04 = 1;
        object->field_06 = 7;
        object->field_05 = 0;
        object->field_07 = 0;
        object->field_15a = 4;
        if (object->side != 0) {
            scratch_call_right(object);
        } else {
            scratch_call_left(object);
        }
    }
}
