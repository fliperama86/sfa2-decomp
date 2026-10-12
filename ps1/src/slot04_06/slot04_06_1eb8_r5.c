/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);
void apply_vertical_velocity_slot04_06(Object *obj);

void update_rising_arc_then_dive_sound_8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    apply_vertical_velocity_slot04_06(o);
    if (o->vel_y <= 0x1ffff && obj->field_1c8 == 0) {
        obj->field_1c8 = 1;
        set_sequence_by_side_and_mode(o, 0x28);
    } else if (o->vel_y >= 0) {
        move_x_by_vel_x_facing_then_accelerate_slot04_06(o);
        update_sequence(o);
    } else {
        o->vel_y = -0x80000;
        o->state_3++;
        play_side_sound_at(o, o->side, 8);
        set_sequence_by_side_and_mode(o, 0x20);
    }
}

void update_fall_to_saved_y_and_land_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    apply_vertical_velocity_slot04_06(o);
    if (o->pos_y < (s16)obj->saved_pos_y) {
        move_x_by_vel_x_facing_then_accelerate_slot04_06(o);
    } else {
        o->state_3++;
        func_80146960(o);
        o->active_state = 0;
        play_sound_at(o, o->side, 0x319);
        obj->field_1c2 = 0xa;
        o->pos_y = obj->saved_pos_y;
        game_state.state_param = 0xa;
        set_sequence_by_side_and_mode(o, 0x21);
    }
}
