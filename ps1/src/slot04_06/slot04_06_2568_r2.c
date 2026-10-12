/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);
void apply_vertical_velocity_slot04_06(Object *obj);

void update_rise_then_start_fall_sequence_b_slot04_06(Object *obj) {

    apply_vertical_velocity_slot04_06(obj);
    if (obj->vel_y <= 0x1ffff && ((Slot04aObj *)obj)->field_1c8 == 0) {
        ((Slot04aObj *)obj)->field_1c8 = 1;
        set_sequence_by_side_and_mode(obj, 0x29);
    } else if (obj->vel_y >= 0) {
        move_x_by_vel_x_facing_then_accelerate_slot04_06(obj);
        update_sequence(obj);
    } else {
        ((Slot04aObj *)obj)->field_1c2 = 10;
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index == 0) {
            obj->state_3 = 0xf;
            obj->vel_x = 0;
            obj->accel_x = 0;
            play_side_sound_at(obj, obj->side, 5);
            set_sequence_by_side_and_mode(obj, 0x24);
        }
    }
}

void count_down_and_drift_up_fast_slot04_06(Object *obj) {
    s32 a;

    ((Slot04aObj *)obj)->field_1c2 = ((Slot04aObj *)obj)->field_1c2 - 1;
    if (((Slot04aObj *)obj)->field_1c2 & 0x80) {
        obj->state_3 = obj->state_3 + 1;
        ((Slot04aObj *)obj)->field_1c2 = 0x28;
    }
    a = 0x4000;
    if (obj->facing_side == 0) {
        a = -0x4000;
    }
    *(s32 *)&obj->pos_x_frac += a;
    *(s32 *)&obj->pos_y_frac += -0x10000;
    update_sequence(obj);
}
