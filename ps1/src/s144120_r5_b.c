/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void advance_state_2_on_sequence_end_set_depth(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->pos_x = 0xc0;
        object->state_2 = object->state_2 + 1;
        set_sequence_from_effect_table(object, 0x10);
        func_80144938(object);
        object->field_01 = 1;
        object->spawn_x = 0x80;
        object->ot_depth = 1;
    } else {
        object->field_01 = 2;
    }
}

void func_80144938(Object *object) {
    if (object->step_flags & 0x80) {
        object->countdown = 7;
        object->vel_x = 0x8000;
        object->vel_y = -0x10000;
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
    }
}

void move_xy_until_countdown_state_2(Object *object) {
    update_position_by_velocity(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        object->state_2 = object->state_2 + 1;
    }
}
