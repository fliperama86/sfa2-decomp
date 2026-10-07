/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015dcc_slot01[];
extern Object *data_80033b98_slot01;
void func_80014bd8_slot01(Object *obj);

void func_80014794_slot01(Object *object) {
    data_80015dcc_slot01[object->field_05](object);
}

void func_800147d4_slot01(Object *obj) {
    Object *p;
    data_80033b98_slot01 = obj->field_3c;
    obj->field_01 = 0;
    obj->field_0c = 1;
    obj->field_05++;
    obj->field_0d = data_80033b98_slot01->field_0d;
    obj->field_0e = 0;
    obj->field_09 = 0;
    obj->field_0f = 0;
    obj->field_46 = 0x40;
    obj->field_26 = 0;
    obj->field_5c = 0xff;
    p = data_80033b98_slot01;
    obj->field_7a = p->field_7a;
    obj->field_7c = p->field_7c;
    if (p->side != 0) {
        obj->field_0b = 0;
    } else {
        obj->field_0b = 1;
    }
    obj->field_03 = obj->field_48 = data_80033b98_slot01->kind;
    func_80014bd8_slot01(obj);
}
