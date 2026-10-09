/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b079c_slot04_05(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) == 0) {
            if (obj->field_0b != 0) {
                *(u32 *)&obj->field_10 = *(u32 *)&obj->field_10 + obj->field_4c;
            } else {
                *(u32 *)&obj->field_10 = *(u32 *)&obj->field_10 - obj->field_4c;
            }
            obj->field_4c = obj->field_4c + obj->field_54;
            if (obj->field_4c < 0) {
                obj->field_4c = 0;
                obj->field_54 = 0;
            }
        }
        func_80130efc(obj);
    }
}
