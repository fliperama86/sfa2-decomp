/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void check_parent_state_key_or_set_state_3_slot00(Object *obj) {
    if ((((Slot00Obj *)obj->parent)->state_key & 0xffffff) == 0x80001) {
        set_flag_and_push_if_mode_8(obj);
    } else {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}

void decrement_parent_counter_and_release_slot00(Object *obj) {
    Object *p = obj->parent;
    p->counter--;
    reset_and_push_block_object_type_8((Slab172 *)obj);
}
