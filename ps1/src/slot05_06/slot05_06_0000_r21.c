/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
void apply_vertical_velocity_slot05_06(Object *obj);
extern ObjectFn data_801dd368_slot05_06[];

void launch_other_hit_on_sequence_end_slot05_06(Object *o) {
    Object *p;
    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        p = o->other;
        p->field_15b = 1;
        set_other_hit_reaction_state_0(o, 1, 0xf, -0x200, 0, 0, 0);
        if ((s16)p->health < 0) {
            if (o->field_49 == 0) {
                o->field_167 = 2;
            } else {
                o->field_167 = 0x16;
            }
        }
        o->vel_x = -0x20000;
        o->accel_y = -0x4800;
        o->vel_y = 0x44000;
        o->accel_x = 0;
        o->active_state = 1;
        o->state_3++;
        set_sequence_by_side_and_mode(o, 0x26);
    }
}

void update_air_motion_or_land_clear_flag_4_slot05_06(Object *o) {
    apply_vertical_velocity_slot05_06(o);
    if (o->pos_y < o->ground_y) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(o);
    } else {
        o->active_state = 0;
        play_object_sound_by_kind(o);
        o->active_flag_4 = 0;
        o->pos_y = o->ground_y;
        reset_object_to_state_1_0_sequence_0(o);
    }
}

void func_801c9c1c_slot05_06(Object *obj) {
    data_801dd368_slot05_06[obj->state_3](obj);
}
