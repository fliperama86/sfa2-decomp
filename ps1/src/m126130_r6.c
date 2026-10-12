/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012a804(Object *object) {
    int index;
    s32 speed;
    if ((u8)is_facing_differs_from_saved(object) == 0) {
        if (object->direction_flag == object->variant_index) {
            *(s32 *)&object->pos_x_frac += object->vel_x;
            update_sequence(object);
            return;
        }
        object->variant_index = object->direction_flag;
    }
    object->facing_side = object->saved_facing;
    index = 2;
    if (object->variant_index != 0) {
        index = 3;
    }
    set_sequence_by_side(object, index);
    speed = motion_speed_table[object->kind * 2 + (object->variant_index & 1)];
    if (object->facing_side != 0) {
        speed = -speed;
    }
    object->vel_x = speed;
    func_8012a684(object);
}
