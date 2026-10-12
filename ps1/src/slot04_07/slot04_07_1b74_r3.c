/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_by_velocity_return_new_vel_y_slot04_07(Object *obj);

void update_fall_and_land_with_damage_slot04_07(Object *obj) {
    int k;

    if (((Slot04aObj *)obj)->mode != 0) {
        update_sequence(obj);
    } else if (move_by_velocity_return_new_vel_y_slot04_07(obj) >= 0 || (k = obj->ground_y) > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = k;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        game_state.state_param = 0x40;
        play_side_sound_at(obj, obj->side, 8);
        k = obj->field_49 != 0 ? 0x14 : 0x11;
        if ((u8)apply_damage_to_other_return_defeated(obj, k, 0) != 0) {
            play_sound_at(obj, obj->side, 0x319);
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x17;
                obj->field_255 = 6;
                game_state.spawn_kind = 3;
                spawn_game_state_kind_object(obj);
            }
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
        set_sequence_by_side_and_mode(obj, 0x22);
    }
}
