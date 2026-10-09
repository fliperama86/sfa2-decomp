/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c61c8_slot04_02[])(Object *);

void func_801b065c_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b06a0_slot04_02(Object *obj) {
    obj->field_157 = 1;
    data_801c61c8_slot04_02[obj->field_07](obj);
}
