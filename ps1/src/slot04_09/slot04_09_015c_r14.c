/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b22d8_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}
