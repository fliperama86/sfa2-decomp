/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8002c7b0_slot28[];

void func_80015c9c_slot28(Object *obj) {
    data_8002c7b0_slot28[obj->field_05](obj);
    obj->field_01 = 1;
    func_80131094(obj);
}

void func_80015cf8_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_05++;
        func_80130768(obj, 0xf, (SequenceStep **)obj->box_tables);
    }
}
