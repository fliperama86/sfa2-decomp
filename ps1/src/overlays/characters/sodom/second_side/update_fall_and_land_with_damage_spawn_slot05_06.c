/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern u8 data_801dd40c_slot05_06[];
void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
void apply_vertical_velocity_slot05_06(Object *obj);
void negate_value_if_facing_side_zero_slot05_06(Object *obj, u16 *p);

void update_fall_and_land_with_damage_spawn_slot05_06(Object *obj) {
    s16 t;
    GameState *g = &game_state;

    apply_vertical_velocity_slot05_06(obj);
    if (obj->pos_y < obj->ground_y) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        spawn_two_effects_at_other_ground(obj);
        if ((s8)spawn_object_variant_5_from_object(obj) != 0) {
            t = 0x80;
            negate_value_if_facing_side_zero_slot05_06(obj, (u16 *)&t);
            ref_other.p->pos_x += t;
            ref_other.p->pos_y -= 0x10;
            if ((s8)spawn_object_variant_5_from_object(obj) != 0) {
                t = 0x40;
                negate_value_if_facing_side_zero_slot05_06(obj, (u16 *)&t);
                ref_other.p->pos_x += t;
                ref_other.p->pos_y -= 0x10;
            }
        }
        obj->pos_y = obj->ground_y;
        obj->active_state = 0;
        ((Slot04aObj *)obj)->field_1c2 = 0xc;
        if (obj->direction_index != 4) {
            ((Slot04aObj *)obj)->field_1c2 = 0x1c;
        }
        g->state_param = 0x1c;
        if ((u8)apply_damage_to_other_return_defeated(obj, *(s16 *)(data_801dd40c_slot05_06 + (obj->direction_index & 0xfe)), 0) != 0 && obj->direction_index != 4) {
            g->spawn_kind = 4;
            spawn_game_state_kind_object(obj);
            play_sound_at(obj, obj->side, 0x319);
            ((Slot04aObj *)obj)->field_1c2 = 0x30;
            g->state_param = 0x20;
            set_sequence_by_side_and_mode(obj, 0x25);
        } else {
            play_sound_at(obj, obj->side, 0x319);
            set_sequence_by_side_and_mode(obj, 0x25);
        }
    }
}
