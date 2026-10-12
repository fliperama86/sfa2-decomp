/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801f67f0_slot06_10;
extern s16 data_801f67f4_slot06_10;

void store_ordered_pos_x_by_facing_side_slot06_10(Object *obj, Object *other) {
    if (obj->facing_side != 0) {
        data_801f67f0_slot06_10 = other->pos_x;
        data_801f67f4_slot06_10 = obj->pos_x;
    } else {
        data_801f67f0_slot06_10 = obj->pos_x;
        data_801f67f4_slot06_10 = other->pos_x;
    }
}
