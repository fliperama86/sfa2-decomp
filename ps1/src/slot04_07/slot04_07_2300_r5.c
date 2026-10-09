/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2994_slot04_07(Object *obj);

void func_801b293c_slot04_07(Object *obj) {
    func_80130efc(obj);
    func_801b2994_slot04_07(obj);
    obj->other->field_249 = 5;
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    }
}

void func_801b2994_slot04_07(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
}
