/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3248_slot04_0a(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_80120554(obj, obj->side, 0x320);
        func_801307e0(obj, 0x1b);
    }
}

void func_801b32b4_slot04_0a(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_159 = 0;
        func_801312b8(obj);
    }
}
