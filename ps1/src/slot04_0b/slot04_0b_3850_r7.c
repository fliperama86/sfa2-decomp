/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4638_slot04_0b(Object *o);

void func_801b454c_slot04_0b(Object *o) {
    o->ot_depth = 1;
    o->state_0++;
    o->facing_side = ref_other.p->facing_side;
    o->field_0c = ref_other.p->field_0c;
    o->image_row = ref_other.p->image_row;
    o->field_0e = ref_other.p->field_0e;
    o->field_0f = ref_other.p->field_0f;
    o->field_1c = ref_other.p->field_1c;
    o->scroll_factor = ref_other.p->scroll_factor;
    if (o->table_index != 0) {
        o->image_row++;
    }
    func_801b4638_slot04_0b(o);
}
