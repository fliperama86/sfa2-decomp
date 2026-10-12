/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028120_slot27[])(Object *);
void init_object_with_two_child_objects_slot27(Object *obj);
void update_sequence_on_active_state_change_slot27(Object *obj);
void func_80015070_slot27(Object *obj);

void func_80014f30_slot27(Object *obj) {
    data_80028120_slot27[obj->state_1](obj);
}

void func_80014f70_slot27(Object *obj) {
    if (ref_other.p->state_0 >= 2) {
        obj->state_0 = obj->state_0 + 1;
    }
    update_sequence_on_active_state_change_slot27(obj);
    func_80015070_slot27(obj);
    func_8011ffdc(obj);
}

void func_80014fd4_slot27(Object *obj) {
    obj->state_1 = 0;
    init_object_with_two_child_objects_slot27(obj);
}

void advance_state_and_set_linked_state_2_slot27(Object *o) {
    o->state_0 = o->state_0 + 1;
    ref_first.p = o->linked_object;
    if (ref_first.p != 0) {
        ref_first.p->state_0 = 2;
    }
    ref_first.p = ((Slot27Obj *)o)->linked_object;
    if (ref_first.p != 0) {
        ref_first.p->state_0 = 2;
    }
}

void func_80015050_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
