/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001da50_slot28(Object *obj);

void func_8001d758_slot28(Object *obj) {
    int kind = obj->field_03;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04 = obj->field_04 + 1;
    if (kind != 2) {
        if (kind == 3) {
            func_8001da50_slot28(obj);
        } else {
            func_80130768(obj, kind, (SequenceStep **)obj->box_tables);
        }
    } else {
        obj->field_09 = 1;
        func_80130768(obj, 6, (SequenceStep **)obj->box_tables);
    }
}
