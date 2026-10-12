/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001da50_slot28(Object *obj);

void func_8001d758_slot28(Object *obj) {
    int kind = obj->table_index;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0 = obj->state_0 + 1;
    if (kind != 2) {
        if (kind == 3) {
            func_8001da50_slot28(obj);
        } else {
            func_80130768(obj, kind, (SequenceStep **)obj->box_tables);
        }
    } else {
        obj->ot_depth = 1;
        func_80130768(obj, 6, (SequenceStep **)obj->box_tables);
    }
}
