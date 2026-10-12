/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c4598_slot04_04[])(Object *);

void advance_state_0_on_sequence_loop_flag_slot04_04(Object *o) {
    if ((s16)o->step_flags < 0) {
        o->state_0 = o->state_0 + 1;
    }
    advance_sequence_checked(o);
}

void release_object_and_unlink_from_parent_slot04_04(Object *o) {
    Object *p = o->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void call_handler_by_state_0_2_slot04_04(Object *obj) {
    data_801c4598_slot04_04[obj->state_0](obj);
}
