/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern s16 data_801dd358_slot05_06[];
extern u8 data_801dd364_slot05_06[];
void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
void apply_vertical_velocity_slot05_06(Object *obj);

void update_fall_and_land_with_damage_slot05_06(Object *o) {
    u8 t;
    GameState *g;
    apply_vertical_velocity_slot05_06(o);
    g = &game_state;
    if (o->pos_y < o->ground_y) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(o);
        update_sequence(o);
    } else {
        o->state_3++;
        spawn_two_effects_at_other_ground(o);
        t = o->direction_index >> 1;
        if (o->field_49 != 0) {
            t += 3;
        }
        if ((u8)apply_damage_to_other_return_defeated(o, data_801dd358_slot05_06[t], 0) != 0) {
            play_sound_at(o, o->side, 0x319);
            if (o->field_49 != 0) {
                o->field_255 = 6;
                game_state.spawn_kind = 0;
                spawn_game_state_kind_object(o);
            }
        } else {
            play_sound_at(o, o->side, 0x319);
        }
        o->pos_y = o->ground_y;
        o->active_state = 0;
        ((Slot04aObj *)o)->field_1c2 = 0x10;
        g->state_param = data_801dd364_slot05_06[o->direction_index >> 1];
        set_sequence_by_side_and_mode(o, 0x21);
    }
}

void count_down_byte_advance_state_3_update_sequence_slot05_06(Object *o) {
    ((Slot04aObj *)o)->field_1c2 += 0xff;
    if (((Slot04aObj *)o)->field_1c2 & 0x80) {
        o->state_3++;
    }
    update_sequence(o);
}
