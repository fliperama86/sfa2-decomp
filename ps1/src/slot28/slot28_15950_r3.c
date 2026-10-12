/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80026368_slot28(Object *obj);

void func_8002603c_slot28(Object *obj) {
    int t = obj->table_index;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->state_0 = obj->state_0 + 1;
    switch (t) {
    case 3:
        func_80026368_slot28(obj);
        break;
    default:
        func_80130768(obj, t != 0 ? 3 : 1, (SequenceStep **)obj->box_tables);
        break;
    case 2:
        obj->ot_depth = 1;
        func_80130768(obj, 3, (SequenceStep **)obj->box_tables);
        break;
    }
}
