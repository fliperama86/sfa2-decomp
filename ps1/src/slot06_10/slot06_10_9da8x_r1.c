/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801f67f0_slot06_10;
extern s16 data_801f67f4_slot06_10;

void func_801e9da8_slot06_10(Object *obj, Object *other) {
    if (obj->field_0b != 0) {
        data_801f67f0_slot06_10 = other->pos_x;
        data_801f67f4_slot06_10 = obj->pos_x;
    } else {
        data_801f67f0_slot06_10 = obj->pos_x;
        data_801f67f4_slot06_10 = other->pos_x;
    }
}
