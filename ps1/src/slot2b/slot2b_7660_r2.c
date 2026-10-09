/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077908_slot2b(Object *obj);

void func_800777c8_slot2b(Object *obj) {
    Slot2bObj *o = (Slot2bObj *)obj;
    o->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    if (o->field_3a == 0) {
        obj->field_07++;
        obj->field_10 = 0;
    }
    func_80130efc(obj);
}

void func_80077828_slot2b(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}
