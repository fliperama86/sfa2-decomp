/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;
extern Slot0fRec8508 data_800e8638_slot0f;
extern Slot0fRec8508 data_800e8658_slot0f;
extern Slot0fRec8508 data_800e8668_slot0f;
extern Slot0fRec8508 data_800e8688_slot0f;
extern Slot0fRec8508 data_800e8698_slot0f;
extern ObjectFn data_800e87ec_slot0f[];

void func_800e1fc0_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    Slot0fRec8508 *rec;

    if (obj->field_0b == 0) {
        func_801519b4(&data_800e85f8_slot0f);
    } else {
        func_801519b4(&data_800e8608_slot0f);
    }
    switch (obj->field_0e) {
    case 0:
        rec = &data_800e8638_slot0f;
        break;
    case 1:
        rec = &data_800e8658_slot0f;
        break;
    case 2:
        rec = &data_800e8668_slot0f;
        break;
    case 3:
        rec = &data_800e8698_slot0f;
        break;
    case 4:
        rec = &data_800e8688_slot0f;
        break;
    default:
        return;
    }
    func_801519b4(rec);
}

void func_800e2088_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    func_801519b4(&data_800e8508_slot0f);
    data_800e87ec_slot0f[(s8)obj->field_04](o);
}
