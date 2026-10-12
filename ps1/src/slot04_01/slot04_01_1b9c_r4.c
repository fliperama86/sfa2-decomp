/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801befe8_slot04_01[];

void set_pos_y_from_table_and_run_step_events_slot04_01(Object *object);

void start_vertical_motion_by_direction_index_slot04_01(Object *o) {
    int a;

    o->state_3++;
    spawn_three_child_objects_at_object_spawn(o);
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    a = o->direction_index >> 1;
    o->accel_y = -0x6000;
    o->vel_y = data_801befe8_slot04_01[o->direction_index >> 1];
    set_sequence_by_side_and_mode(o, a + 0x38);
}

void update_sequence_then_advance_state_3_on_flag_slot04_01(Object *o) {
    update_sequence(o);
    if ((u8)o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        set_pos_y_from_table_and_run_step_events_slot04_01(o);
    }
}

void update_sequence_until_end_then_set_sequence_slot04_01(Object *o) {
    update_sequence(o);
    if ((s16)o->step_flags & 0x8000) {
        o->active_state = 1;
        o->state_3++;
        set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x3b);
    } else {
        set_pos_y_from_table_and_run_step_events_slot04_01(o);
    }
}
