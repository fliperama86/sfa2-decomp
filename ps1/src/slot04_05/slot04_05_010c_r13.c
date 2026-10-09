/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3934_slot04_05(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
