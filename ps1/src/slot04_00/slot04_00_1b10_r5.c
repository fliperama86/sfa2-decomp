/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void land_on_ground_set_sequence_slot04_00(Object *obj) {
    if (obj->active_state != 0) {
        play_object_sound_by_kind(obj);
    }
    obj->state_3++;
    obj->pos_y_frac = 0;
    obj->active_state = 0;
    obj->active_flag_3 = 0;
    obj->pos_y = obj->ground_y;
    set_sequence_by_side(obj, 0x11);
}
