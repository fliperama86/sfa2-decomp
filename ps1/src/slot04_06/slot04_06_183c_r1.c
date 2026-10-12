/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);
void apply_vertical_velocity_slot04_06(Object *obj);

void advance_state_3_on_step_flag_byte_slot04_06(Object *o) {
    if (*(u8 *)&o->step_flags != 0) {
        o->active_state = 1;
        ((Slot04aObj *)o)->field_1c8 = 0;
        o->state_3++;
    }
    update_sequence(o);
}

void func_801b1880_slot04_06(Object *o) {
    apply_vertical_velocity_slot04_06(o);
    if (o->vel_y <= 0x1ffff && ((Slot04aObj *)o)->field_1c8 == 0) {
        ((Slot04aObj *)o)->field_1c8 = 1;
        set_sequence_by_side_and_mode(o, 0x28);
    } else if (o->vel_y >= 0) {
        move_x_by_vel_x_facing_then_accelerate_slot04_06(o);
        update_sequence(o);
    } else {
        o->vel_x = 0x18000;
        o->vel_y = -0x80000;
        o->state_3++;
        play_side_sound_at(o, o->side, 4);
        set_sequence_by_side_and_mode(o, 0x20);
    }
}
