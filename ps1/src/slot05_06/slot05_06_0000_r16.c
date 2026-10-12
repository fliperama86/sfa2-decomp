/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_adding_meter_5_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    obj->countdown_2 = 0;
    o->active_flag_4 = 1;
    o->state_3++;
    if (o->field_49 != 0) {
        o->field_225 = 1;
    }
    add_to_meter_clamped(o, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    set_sequence_by_side_and_mode(o, 0x1e);
}

void init_jump_velocity_and_advance_state_3_slot05_06(Object *obj) {
    obj->vel_x = 0x40000;
    obj->vel_y = 0x28000;
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->accel_x = 0;
    obj->accel_y = -0xa000;
    obj->active_state = 1;
    obj->state_3++;
}
