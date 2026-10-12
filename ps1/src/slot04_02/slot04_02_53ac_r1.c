/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6518_slot04_02[];
extern u8 data_801c6520_slot04_02[];

void update_airborne_then_land_and_hit_other_slot04_02(Object *obj) {
    int i;
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0 && obj->pos_y >= obj->ground_y) {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        spawn_two_effects_at_other_ground(obj);
        i = obj->direction_index & 0xfe;
        game_state.state_param = data_801c6518_slot04_02[i];
        if ((u8)apply_damage_to_other_return_defeated(obj, *(s16 *)(data_801c6520_slot04_02 + i), 0) != 0) {
            play_sound_at(obj, obj->side, 0x319);
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x12;
                obj->field_255 = 6;
                game_state.spawn_kind = 0;
                spawn_game_state_kind_object(obj);
            }
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
        set_sequence_by_side_and_mode(obj, 0x59);
    } else {
        update_sequence(obj);
    }
}
