/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c64e8_slot04_02[];

void choose_air_attack_branch_by_button_bits_slot04_02(Object *obj);
void choose_air_attack_branch_by_distance_slot04_02(Object *obj);

void update_airborne_motion_until_landing_a_slot04_02(Object *obj) {
    u16 a;

    update_sequence(obj);
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= obj->ground_y) {
        obj->field_157 = 1;
        obj->active_flag_3 = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        a = obj->direction_index >> 1;
        obj->accel_x = data_801c64e8_slot04_02[a];
        if (obj->field_49 != 0 && obj->mode_flag_2 == 0) {
            a += 0x6d;
        } else {
            a += 0x4d;
        }
        set_sequence_by_side_and_mode(obj, a);
        play_sound_at(obj, obj->side, 0x324);
    } else if (obj->mode_flag_2 != 0) {
        choose_air_attack_branch_by_distance_slot04_02(obj);
    } else {
        choose_air_attack_branch_by_button_bits_slot04_02(obj);
    }
}
