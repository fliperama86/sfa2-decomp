/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_facing_with_decel_stop_slot04_07(Object *obj);

void start_dash_decay_phase_slot04_07(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->state_3 = 2;
    o->countdown = 8;
    o->accel_x = *(s32 *)&obj->field_1c0;
    set_sequence_by_side_and_mode(o, 0x24);
}

void func_801b2338_slot04_07(Object *obj) {
    obj->state_3 = 2;
    obj->field_157 = 1;
    if (obj->field_49 != 0) {
        set_sequence_by_side_and_mode(obj, 0x28);
    } else {
        set_sequence_by_side_and_mode(obj, 0x2c);
    }
}

void set_rising_velocities_and_sequence_by_flag_slot04_07(Object *obj) {
    obj->state_3 = 3;
    obj->active_state = 1;
    obj->vel_x = 0x70000;
    obj->accel_x = -0x1000;
    obj->vel_y = 0x50000;
    obj->accel_y = -0x8000;
    *(u16 *)&obj->pos_y -= 1;
    if (obj->field_49 != 0) {
        set_sequence_by_side_and_mode(obj, 0x27);
    } else {
        set_sequence_by_side_and_mode(obj, 0x2b);
    }
}

int is_compare_limit_below_threshold_slot04_07(Object *obj) {
    int t;

    if (obj->mode_flag_2 == 0) {
        return (s16)obj->compare_limit < 0x20;
    }
    t = 0x30;
    if (obj->direction_index != 0) {
        t = 0x60;
    }
    return (s16)obj->compare_limit < t;
}

void update_sequence_with_slide_step_slot04_07(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0xff) == 0) {
            move_x_by_facing_with_decel_stop_slot04_07(obj);
        }
        update_sequence(obj);
    }
}
