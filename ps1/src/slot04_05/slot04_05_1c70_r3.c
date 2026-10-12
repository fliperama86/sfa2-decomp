/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c14c0_slot04_05[];
extern s32 data_801c14d8_slot04_05[];

void launch_from_table_velocity_on_step_flag_slot04_05(Object *obj) {
    int a;
    int b;
    s32 v = data_801aa5e4[0];

    *(s32 *)&obj->pos_x_frac = v;
    if (obj->facing_side == 0) {
        *(s32 *)&obj->pos_x_frac = v + 0x1800000;
    }
    if (*(u8 *)&obj->step_flags == 1) {
        obj->variant_index = 1;
        obj->state_3++;
        obj->accel_x = 0;
        obj->accel_y = 0;
        a = data_801c14c0_slot04_05[obj->direction_index];
        b = data_801c14c0_slot04_05[obj->direction_index + 1];
        if (obj->field_49 != 0) {
            a = data_801c14d8_slot04_05[obj->direction_index];
            b = data_801c14d8_slot04_05[obj->direction_index + 1];
        }
        if (obj->facing_side == 0) {
            a = -a;
        }
        obj->vel_x = a;
        obj->vel_y = b;
    }
    update_sequence(obj);
}

void update_airborne_or_land_with_kind_sound_slot04_05(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if (*(u8 *)&obj->step_flags != 2) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->pos_y = obj->ground_y;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
    }
}
