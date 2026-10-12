/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e85b8_slot0f;
extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;
extern Slot0fRec8508 data_800e86b8_slot0f;
extern Slot0fRec8508 data_800e86c8_slot0f;
extern ObjectFn data_800e87fc_slot0f[];

void func_800e21f8_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    func_801519b4(&data_800e8508_slot0f);
    data_800e87fc_slot0f[(s8)obj->handler_index](o);
}

void func_800e2254_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Object *first;

    first = (Object *)&data_800e8608_slot0f;
    if (obj->field_0b == 0) {
        first = (Object *)&data_800e85f8_slot0f;
    }
    func_801519b4(first);
    func_801519b4(&data_800e85b8_slot0f);
}

void func_800e22a0_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Slot0fRec8508 *rec;

    if (obj->field_0b == 0) {
        func_801519b4(&data_800e85f8_slot0f);
    } else {
        func_801519b4(&data_800e8608_slot0f);
    }
    switch (obj->mode) {
    case 0:
        rec = &data_800e86b8_slot0f;
        break;
    case 1:
        rec = &data_800e86c8_slot0f;
        break;
    default:
        return;
    }
    func_801519b4(rec);
}

void func_800e2328_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    obj->state_index = 1;
    obj->field_07 = 0;
    obj->field_06 = 0;
    obj->field_05 = 0;
    obj->handler_index = 0;
    obj->handler_index_b = 0;
}
