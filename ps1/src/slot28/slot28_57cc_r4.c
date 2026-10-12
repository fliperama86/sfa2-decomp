/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8002c7a4_slot28[];

void call_state_1_handler_then_advance_sequence_slot28(Object *obj) {
    data_8002c7a4_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
}

void blink_object_and_advance_state_1_on_timeout_slot28(Object *obj) {
    if (--obj->vel_x == 0) {
        obj->vel_x = 0x60;
        obj->state_1++;
    }
    obj->variant_index = obj->variant_index ^ 1;
    obj->field_01 = 0;
    if (obj->variant_index != 0) {
        obj->field_01 = 1;
    }
}

void count_down_vel_x_to_next_state_1_slot28(Object *obj) {
    if (--obj->vel_x == 0) {
        obj->vel_x = 0x20;
        obj->state_1++;
    }
    obj->field_01 = 1;
}
