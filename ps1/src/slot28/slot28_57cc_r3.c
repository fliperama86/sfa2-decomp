/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_8002c77c_slot28[];
extern ObjectFn data_8002c78c_slot28[];
extern ObjectFn data_8002c79c_slot28[];
void func_80015d3c_slot28(Object *obj);

void func_8001591c_slot28(Object *obj) {
    obj->field_4c = 0x10;
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04++;
    func_80130768(obj, data_8002c77c_slot28[obj->field_03], (SequenceStep **)obj->box_tables);
}

void func_80015978_slot28(Object *obj) {
    data_8002c78c_slot28[obj->field_03](obj);
}

void func_800159b8_slot28(Object *obj) {
    data_8002c79c_slot28[obj->field_05](obj);
    func_80131094(obj);
    func_80120028(obj);
}

void func_80015a14_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_05++;
        obj->field_0c = 0;
        func_80130768(obj, 4, (SequenceStep **)obj->box_tables);
    }
    func_80015d3c_slot28(obj);
}
