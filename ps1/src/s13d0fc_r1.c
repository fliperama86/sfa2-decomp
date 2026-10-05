/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8013d0fc(Object *object) {
    int bits = (object->field_134 | object->field_136) & 0x6a;

    if (bits != 0x68) {
        return bits == 2;
    }
    return 1;
}

u8 func_8013d130(Object *object, u8 index, u8 arg) {
    return func_8013e190(object, index, arg);
}

u8 func_8013d158(Object *object, u8 index, u8 arg) {
    return func_8013e3b4(object, index, arg);
}

u8 func_8013d180(Object *object, u8 index, u8 arg) {
    return func_8013d21c(object, index, arg);
}
