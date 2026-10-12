/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_80079900_slot2b[];
extern s16 data_8007ef24_slot2b;

void halve_vel_x_add_meter_set_sequence_slot2b(Object *obj) {
    obj->vel_x = obj->vel_x >> 1;
    add_to_meter_clamped(obj, 1);
    set_sequence_by_side_and_mode(obj, 0x1a);
}

void call_handler_by_state_0_store_ground_y_slot2b(Object *obj) {
    data_8007ef24_slot2b = (u16)obj->parent->ground_y;
    data_80079900_slot2b[obj->state_0](obj);
}
