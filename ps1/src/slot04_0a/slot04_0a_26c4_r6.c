/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ce0_slot04_0a(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_07 = 0;
        func_801312b8(obj);
    }
}
