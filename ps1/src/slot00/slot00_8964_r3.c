/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void flip_accel_and_depth_on_three_countdowns_slot00(Object *obj) {
    obj->field_60 -= 1;
    if (obj->field_60 & 0x80) {
        obj->field_60 = 0x13;
        obj->accel_x = -obj->accel_x;
        obj->ot_depth ^= 6;
    }
    obj->hit_type -= 1;
    if (obj->hit_type & 0x80) {
        obj->hit_type = 0x1f;
        obj->accel_y = -obj->accel_y;
    }
    obj->active_state -= 1;
    if (obj->active_state & 0x80) {
        obj->active_state = 0x27;
        obj->ot_depth ^= 6;
    }
}
