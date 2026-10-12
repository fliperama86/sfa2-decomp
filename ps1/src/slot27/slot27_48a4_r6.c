/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014dec_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    o->active_state = 0xff;
    o->field_6b = 0;
    if (o->table_index >= 2) {
        ref_other.p = func_8011f32c();
        if (ref_other.p != 0) {
            ref_other.p->active_count = 1;
            ref_other.p->field_02 = 0x1d;
            ref_other.p->table_index = 0;
            ref_other.p->other = (*(Object **)&data_80190458);
            ref_other.p->parent = o;
            obj->other_ref = ref_other.p;
        }
        ref_other.p = func_8011f32c();
        if (ref_other.p != 0) {
            ref_other.p->active_count = 1;
            ref_other.p->field_02 = 0x1d;
            ref_other.p->table_index = 1;
            ref_other.p->other = (*(Object **)&data_80190458);
            ref_other.p->parent = o;
            obj->linked_object = ref_other.p;
        }
    }
    ref_other.p = o->parent;
}
