/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b4be8_slot04_04(Object *obj);

void update_airborne_motion_and_land_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (func_801b4be8_slot04_04(obj) < 0 && obj->ground_y <= obj->pos_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->facing_side = obj->facing_side ^ 1;
        play_object_sound_by_kind(obj);
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
