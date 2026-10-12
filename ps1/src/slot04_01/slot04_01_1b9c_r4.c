/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801befe8_slot04_01[];

void set_pos_y_from_table_and_run_step_events_slot04_01(Object *object);

void func_801b1fa8_slot04_01(Object *o) {
    int a;

    o->state_3++;
    func_80145d20(o);
    func_801428e4(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    a = o->direction_index >> 1;
    o->accel_y = -0x6000;
    o->vel_y = data_801befe8_slot04_01[o->direction_index >> 1];
    set_sequence_by_side_and_mode(o, a + 0x38);
}

void func_801b2034_slot04_01(Object *o) {
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

void func_801b2098_slot04_01(Object *o) {
    update_sequence(o);
    if ((s16)o->step_flags & 0x8000) {
        o->active_state = 1;
        o->state_3++;
        set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x3b);
    } else {
        set_pos_y_from_table_and_run_step_events_slot04_01(o);
    }
}
