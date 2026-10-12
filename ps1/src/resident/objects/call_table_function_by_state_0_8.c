/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_table_function_by_state_0_8(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cef0[object->state_0](object);
}

void set_depth_one_and_advance_state_0(Object *object) {
    object->ot_depth = 1;
    object->field_0c = 0xff;
    object->field_01 = 0;
    object->state_0 = object->state_0 + 1;
    clear_object_side_table_entries(object);
}

void wait_countdown_then_advance_state_0(Object *object) {
    s16 old = object->countdown;
    object->countdown = old - 1;
    if (old < 0) {
        /* Signed store: a plain assignment compiles to ori 0xffff. */
        *(s16 *)&object->countdown = -1;
        if (object->parent->field_7e == 0) {
            object->state_0 = object->state_0 + 1;
        }
        clear_or_set_flag_by_pos_x_limit(object);
    }
}
