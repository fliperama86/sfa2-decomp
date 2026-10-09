/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b46f8_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_17b = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
