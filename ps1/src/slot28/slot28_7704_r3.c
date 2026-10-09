/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80032a4c_slot28[])(Object *);
extern void (*data_80032a54_slot28[])(Object *);

void func_80017b54_slot28(Object *obj) {
    int arg;
    obj->field_46 = 0x1818;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04++;
    arg = 1;
    if (obj->field_03 == 1) {
        arg = 3;
    }
    func_80130768(obj, arg, (SequenceStep **)obj->box_tables);
}

void func_80017bac_slot28(Object *obj) {
    func_80131094(obj);
    data_80032a4c_slot28[obj->field_03](obj);
}

void func_80017bfc_slot28(Object *obj) {
    data_80032a54_slot28[obj->field_05](obj);
    obj->field_01 = 1;
}
