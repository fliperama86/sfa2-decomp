/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b21e0_slot04_0d(Object *obj) {
    s32 a;

    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->ground_y <= obj->pos_y) {
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->state_3++;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x34);
    } else {
        if (obj->facing_side != 0) {
            a = obj->vel_x;
        } else {
            a = -obj->vel_x;
        }
        *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
        update_sequence(obj);
    }
}
