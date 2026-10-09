/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c20e8_slot04_07[])(Object *, Object *);
void func_801b42a4_slot04_07(Object *obj, Object *parent);

int func_801b41c4_slot04_07(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    return obj->field_50 = obj->field_50 + obj->field_58;
}

void func_801b4208_slot04_07(Object *obj) {
    data_801c20e8_slot04_07[obj->field_04](obj, obj->field_3c);
}

void func_801b4248_slot04_07(Object *obj, Object *parent) {
    obj->field_04++;
    obj->field_1c = parent->field_1c;
    obj->field_03 = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->field_48 = 0;
    func_801b42a4_slot04_07(obj, parent);
}
