/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801bf270_slot04_01[])(Object *, Object *);
void follow_parent_frame_param_sequence_slot04_01(Object *obj, Object *parent);

void reset_states_on_negative_step_flags_slot04_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void unlink_from_parent_count_and_release_object_slot04_01(Object *o) {
    Object *p = o->parent;
    u8 n = p->link_flags - 1;
    p->link_flags = n;
    if (n == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void call_handler_by_state_0_with_parent_slot04_01(Object *obj) {
    data_801bf270_slot04_01[obj->state_0](obj, obj->parent);
}

void init_child_from_parent_fields_slot04_01(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    follow_parent_frame_param_sequence_slot04_01(obj, parent);
}
