/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80131468(Object *object);

void func_801b43c8_slot04_02(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->other->field_249 = 5;
        func_80131468(obj);
    } else {
        if ((t & 0xff) == 0) {
            *(s32 *)&obj->field_10 += obj->field_4c;
        }
        func_80130efc(obj);
    }
}
