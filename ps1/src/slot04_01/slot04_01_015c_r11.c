/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bf0d8_slot04_01[];

void func_801b2c10_slot04_01(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2c54_slot04_01(Object *obj) {
    data_801bf0d8_slot04_01[obj->field_07](obj);
}
