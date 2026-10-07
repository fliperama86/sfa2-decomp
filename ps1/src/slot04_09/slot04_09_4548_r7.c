/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);

void func_801b5c6c_slot04_09(Object *obj);

void func_801b4d84_slot04_09(Object *obj) {
    s16 t = obj->field_3a;
    if (t & 0x8000) {
        obj->field_10 = 0;
        func_801312b8(obj);
    } else {
        if ((u8)t != 0) {
            obj->field_17b = 0;
            func_80142adc(obj);
        } else {
            func_801b5c6c_slot04_09(obj);
        }
        func_80130efc(obj);
    }
}
