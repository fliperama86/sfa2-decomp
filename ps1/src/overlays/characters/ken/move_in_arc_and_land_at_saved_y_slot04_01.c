/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_in_arc_and_land_at_saved_y_slot04_01(Object *obj) {
    s32 t = obj->vel_y + obj->accel_y;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - t;
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_y = t;
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side(obj, 0x11);
    }
}
