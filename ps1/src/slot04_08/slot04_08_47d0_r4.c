/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4c20_slot04_08(Object *obj, Object *other) {
    if (other->field_15a != 0 || other->field_06 != 7 || other->field_07 == 4) {
        obj->field_04++;
    }
    *(s32 *)&obj->field_10 = *(s32 *)&other->field_10;
    func_80131094(obj);
    obj->field_01 ^= 1;
}

void func_801b4ca0_slot04_08(Object *obj, Object *unused) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_04++;
    }
    func_80131094(obj);
    obj->field_01 = 1;
}
