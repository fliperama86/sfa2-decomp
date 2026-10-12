/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800374a4_slot28[])(Object *);

void ignore_object_empty_19_slot28(Object *obj) {
}

void wait_timer_reload_then_advance_state_1_2_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->timer_reload--;
    if (obj->timer_reload == 0) {
        o->state_1++;
    }
}

void call_state_1_handler_d_and_show_5_slot28(Object *obj) {
    data_800374a4_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
