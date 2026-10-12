/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c535c_slot04_06[];
extern u8 data_801c5368_slot04_06[];
void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);
void apply_vertical_velocity_slot04_06(Object *obj);

void update_fall_and_land_with_damage_slot04_06(Object *o) {
    u8 t;
    GameState *g;
    apply_vertical_velocity_slot04_06(o);
    g = &game_state;
    if (o->pos_y < o->ground_y) {
        move_x_by_vel_x_facing_then_accelerate_slot04_06(o);
        update_sequence(o);
    } else {
        o->state_3++;
        func_80146960(o);
        t = o->direction_index >> 1;
        if (o->field_49 != 0) {
            t += 3;
        }
        if ((u8)apply_damage_to_other_return_defeated(o, data_801c535c_slot04_06[t], 0) != 0) {
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
        g->state_param = data_801c5368_slot04_06[o->direction_index >> 1];
        set_sequence_by_side_and_mode(o, 0x21);
    }
}

void func_801b1a88_slot04_06(Object *o) {
    ((Slot04aObj *)o)->field_1c2 += 0xff;
    if (((Slot04aObj *)o)->field_1c2 & 0x80) {
        o->state_3++;
    }
    update_sequence(o);
}
