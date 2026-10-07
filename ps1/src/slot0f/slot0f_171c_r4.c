/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800e87a8_slot0f[])(Object *);
int func_800e0d30_slot0f(void);
void func_800e2348_slot0f(Object *o);

void func_800e1a98_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    if (o->field_10 & 0x860) {
        obj->field_0f = 1;
    }
    obj->field_0f -= 1;
    if (obj->field_0f == 0) {
        o->field_01 = 0;
        o->field_04 = 0;
        if (obj->field_0e == 4) {
            o->field_00 = 4;
            o->field_09 = 3;
        }
    }
}

void func_800e1af4_slot0f(Object *o) {
    data_800e87a8_slot0f[(s8)o->field_01](o);
}

void func_800e1b34_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    o->field_01 += 1;
    o->field_0d = func_800e0d30_slot0f();
    o->field_09 = 2;
    obj->field_0b = 0;
    func_800e2348_slot0f(o);
}
