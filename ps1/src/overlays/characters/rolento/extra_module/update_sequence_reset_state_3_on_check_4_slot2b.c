/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_8007985c_slot2b[];

void update_sequence_reset_state_3_on_check_4_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_5_slot2b(Object *obj) {
    data_8007985c_slot2b[obj->state_3](obj);
}

void set_horizontal_velocity_by_saved_facing_slot2b(Object *obj) {
    obj->active_flag_3 = 1;
    obj->vel_x = 0xa0000;
    obj->state_3++;
    obj->accel_x = 0xffff4000;
    obj->facing_side = obj->saved_facing;
    if (obj->saved_facing == 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    set_sequence_and_tables_from_direction(obj);
}

void play_sound_advance_state_3_on_active_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    if (obj->active_flag != 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x324);
    }
    update_sequence(o);
}
