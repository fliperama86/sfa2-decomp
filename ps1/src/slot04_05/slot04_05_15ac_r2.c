/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1454_slot04_05[];

void update_airborne_with_table_gravity_and_landing_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y = obj->accel_y - data_801c1454_slot04_05[obj->direction_index >> 1];
    }
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if (obj->step_flags & 0x80) {
            return;
        }
    } else {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
    }
    update_sequence(obj);
}
