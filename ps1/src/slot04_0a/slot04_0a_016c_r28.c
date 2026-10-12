/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0840_slot04_0a[])(Object *);

void call_handler_by_state_1_slot04_0a(Object *obj) {
    data_801c0840_slot04_0a[obj->state_1](obj);
}

void load_velocities_from_table_6_slot04_0a(Object *obj) {
    update_object_motion_unless_paused(obj);
}

void load_velocities_from_table_7_slot04_0a(Object *obj) {
    call_state_2_handler_267c_unless_paused(obj);
}

void func_801b41e0_slot04_0a(Object *o) {
    call_state_2_handler_2684_unless_paused(o);
}

void load_velocities_from_table_8_slot04_0a(Object *obj) {
    call_state_2_handler_268c_unless_paused(obj);
}

void load_velocities_from_table_9_slot04_0a(Object *obj) {
    call_handler_2694_by_state_2(obj);
}
