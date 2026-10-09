/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c14fc_slot04_05[];
extern ObjectFn data_801c1508_slot04_05[];

void func_801b2100_slot04_05(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2144_slot04_05(Object *obj) {
    data_801c14fc_slot04_05[obj->field_12a >> 1](obj);
}

void func_801b2188_slot04_05(Object *obj) {
    data_801c1508_slot04_05[obj->field_07](obj);
}
