/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800353a0_slot28[])(Object *);

void advance_state_0_and_init_upward_velocity_slot28(Object *obj) {
    obj->state_0++;
    obj->field_0c = 0;
    obj->field_0e = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    if (obj->table_index == 4) {
        obj->vel_y = 0xfffec000;
        obj->accel_y = 0x1000;
    }
    if (obj->table_index == 5) {
        obj->vel_y = 0xfffee000;
        obj->accel_y = 0x1000;
    }
}

void call_table_handler_by_table_index_slot28(Object *obj) {
    data_800353a0_slot28[obj->table_index](obj);
    obj->field_01 = 1;
}
