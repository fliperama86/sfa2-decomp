/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e8568_slot0f;
extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;

extern ObjectFn data_800e87dc_slot0f[];
void func_800e2498_slot0f(Object *o);
void func_800e25e0_slot0f(Object *o);

void func_800e1ed8_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    func_801519b4(&data_800e8508_slot0f);
    data_800e87dc_slot0f[(s8)obj->field_04](o);
}

void func_800e1f34_slot0f(Object *object) {
    func_800e2498_slot0f(object);
}

void func_800e1f54_slot0f(Object *object) {
    func_800e25e0_slot0f(object);
}

void func_800e1f74_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Object *first;

    first = (Object *)&data_800e8608_slot0f;
    if (obj->field_0b == 0) {
        first = (Object *)&data_800e85f8_slot0f;
    }
    func_801519b4(first);
    func_801519b4(&data_800e8568_slot0f);
}
