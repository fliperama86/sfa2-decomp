/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079900_slot2b[];
extern s16 data_8007ef24_slot2b;

void func_80077908_slot2b(Object *obj) {
    obj->field_4c = obj->field_4c >> 1;
    func_80141f28(obj, 1);
    func_801307e0(obj, 0x1a);
}

void func_8007794c_slot2b(Object *obj) {
    data_8007ef24_slot2b = (u16)obj->field_3c->field_70;
    data_80079900_slot2b[obj->field_04](obj);
}
