/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7cc0_slot04_09[];

void apply_velocity_and_acceleration_slot04_09(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->pos_y_hi -= o->vel_y;
    o->vel_y += o->accel_y;
    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
}

void call_table_function_by_state_3_8_slot04_09(Object *obj) {
    data_801c7cc0_slot04_09[obj->state_3](obj);
}
