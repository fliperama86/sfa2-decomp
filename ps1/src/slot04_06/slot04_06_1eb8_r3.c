/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_80142adc(Object *object);

void func_801b2168_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_3a != 0) {
        o->field_07++;
        obj->field_3a = 0;
    }
    func_80130efc(o);
}

void func_801b21a8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_3b & 0x80) {
        o->other->field_249 = 5;
        func_801312b8(o);
    } else {
        if (obj->field_3a != 0) {
            obj->field_3a = 0;
            o->field_17b = 0;
        }
        if (o->field_4c >= 0) {
            func_801b4818_slot04_06(o);
        }
        func_80142adc(o);
        func_80130efc(o);
    }
}
