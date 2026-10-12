/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028120_slot27[])(Object *);
void init_object_with_two_child_objects_slot27(Object *obj);
void update_sequence_on_active_state_change_slot27(Object *obj);
void set_position_from_other_plus_table_offset_slot27(Object *obj);

void func_80014f30_slot27(Object *obj) {
    data_80028120_slot27[obj->state_1](obj);
}

void follow_other_position_and_advance_state_0_slot27(Object *obj) {
    if (ref_other.p->state_0 >= 2) {
        obj->state_0 = obj->state_0 + 1;
    }
    update_sequence_on_active_state_change_slot27(obj);
    set_position_from_other_plus_table_offset_slot27(obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void reset_state_1_and_init_two_child_objects_slot27(Object *obj) {
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

void release_object_to_pool_table_7_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
