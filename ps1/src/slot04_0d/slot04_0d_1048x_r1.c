/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142778(Object *object);

int func_801b1048_slot04_0d(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) goto z;
    if ((u8)func_80141788(obj)) goto b;
z:
    return 0;
b:
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_15a = 8;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142778(obj);
    return 1;
}
