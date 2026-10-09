/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *obj);
void func_801b4868_slot04_0a(Object *obj);
void func_801b4948_slot04_0a(Object *obj);

void func_801b47c0_slot04_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4828_slot04_0a(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 == 0) {
        func_801b4868_slot04_0a(obj);
    } else {
        func_801b4948_slot04_0a(obj);
    }
}
