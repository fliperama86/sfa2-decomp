/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2898_slot04_01(Object *obj) {
    u8 t = obj->field_07;

    obj->field_17b = 1;
    obj->field_29c = 2;
    obj->field_159 = 1;
    obj->field_45 = 1;
    t++;
    obj->field_07 = t;
    func_80138ae8(&game_state, obj);
    if (obj->field_0b != 0) {
        obj->field_4c = 0x40000;
    } else {
        obj->field_4c = -0x40000;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x35);
}

void func_801b291c_slot04_01(Object *obj) {
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
