/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_object_with_two_child_objects_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    o->active_state = 0xff;
    o->field_6b = 0;
    if (o->table_index >= 2) {
        ref_other.p = pop_object_from_second_pool();
        if (ref_other.p != 0) {
            ref_other.p->active_count = 1;
            ref_other.p->field_02 = 0x1d;
            ref_other.p->table_index = 0;
            ref_other.p->other = (*(Object **)&ref_aux);
            ref_other.p->parent = o;
            obj->other_ref = ref_other.p;
        }
        ref_other.p = pop_object_from_second_pool();
        if (ref_other.p != 0) {
            ref_other.p->active_count = 1;
            ref_other.p->field_02 = 0x1d;
            ref_other.p->table_index = 1;
            ref_other.p->other = (*(Object **)&ref_aux);
            ref_other.p->parent = o;
            obj->linked_object = ref_other.p;
        }
    }
    ref_other.p = o->parent;
}
