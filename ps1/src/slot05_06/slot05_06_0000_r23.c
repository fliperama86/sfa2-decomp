/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd378_slot05_06[];
void set_vel_x_from_direction_table_slot05_06(Object *obj);
void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);

void func_801c9cf4_slot05_06(Object *o) {
    u8 t;
    if (((Slot04aObj *)o)->countdown_2 == 0x2f) {
        t = 1;
        if (o->field_4b == 0) {
            t = 0xff;
        }
        o->field_165 = t;
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -2, 0x5c);
    }
    if (((Slot04aObj *)o)->countdown_2 != 0) {
        ((Slot04aObj *)o)->countdown_2--;
        t = 0;
        if (((Slot04aObj *)o)->countdown_2 != 0) {
            return;
        }
        o->field_165 = 0;
        if (o->field_4b == 0) {
            o->other->field_6b = 10;
            t = (o->direction_index >> 1) + 1;
        }
        o->other_lock = data_801dd378_slot05_06[t];
    }
    if ((s16)o->step_flags >= 0) {
        if (*(u8 *)&o->step_flags != 0) {
            *(u8 *)&o->step_flags = 0;
            set_vel_x_from_direction_table_slot05_06(o);
        }
        if (o->vel_x >= 0) {
            move_x_by_vel_x_facing_then_accelerate_slot05_06(o);
        }
        update_sequence(o);
    } else if (o->direction_index == 4) {
        o->active_state = 1;
        o->vel_y = 0xb0000;
        o->accel_y = -0xe000;
        o->state_3++;
        if (o->facing_side == 0) {
            o->vel_x = -0x40000;
            o->accel_x = 0x500;
        } else {
            o->vel_x = 0x40000;
            o->accel_x = -0x500;
        }
        o->countdown = 0;
        play_side_sound_at(o, o->side, 6);
        set_sequence_by_side_and_mode(o, 0x37);
    } else {
        reset_object_to_state_1_0_sequence_0(o);
    }
}
