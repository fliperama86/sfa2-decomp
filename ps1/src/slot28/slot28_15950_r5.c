/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8002629c_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_05 = obj->field_05 + 1;
        func_80130768(obj, 4, (SequenceStep **)obj->box_tables);
    }
}
