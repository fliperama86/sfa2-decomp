/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void enter_state_7_if_airborne_conditions(Object *object) {
    if (object->mode_flag_2 != 0 && object->table_column >= 0x1d &&
        game_state.loop_counter == 0 &&
        (object->active_flag != 0 || ((s16)object->step_flags & 0x8000)) &&
        object->linked_object_3 == 0 &&
        object->pos_y < (s16)(object->ground_y - 0x30)) {
        object->state_0 = 1;
        object->state_2 = 7;
        object->state_1 = 0;
        object->state_3 = 0;
        object->field_15a = 4;
        if (object->side != 0) {
            scratch_call_right(object);
        } else {
            scratch_call_left(object);
        }
    }
}
