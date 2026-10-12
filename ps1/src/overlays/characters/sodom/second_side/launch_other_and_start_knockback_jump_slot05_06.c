/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *object);
void apply_vertical_velocity_slot05_06(Object *object);

void launch_other_and_start_knockback_jump_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *other;
    GameState *g = &game_state;

    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        other = o->other;
        other->field_15b = 1;
        if (obj->table_value < 2 || (obj->table_value & 0x80)) {
            set_other_hit_reaction_state_0(o, 0, 0xc, 3, 0, 0, 0);
        } else {
            set_other_hit_reaction_state_0(o, 0, 0xa, 3, 0, 0, 0);
        }
        if ((s16)other->health < 0) {
            o->field_167 = 2;
            if (o->field_49 != 0) {
                o->field_167 = 0x16;
                o->field_255 = 6;
                g->spawn_kind = 0;
                spawn_game_state_kind_object(o);
            }
        }
        other->step_applied_flag = 0;
        o->vel_x = 0xfffe0000;
        o->accel_y = -0x4800;
        o->vel_y = 0x44000;
        o->accel_x = 0;
        o->active_state = 1;
        o->state_3++;
        set_sequence_by_side_and_mode(o, 0x26);
    }
}

void update_air_motion_or_land_inclusive_slot05_06(Object *obj) {
    apply_vertical_velocity_slot05_06(obj);
    if (obj->pos_y <= obj->ground_y) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
    } else {
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        obj->active_flag_4 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
