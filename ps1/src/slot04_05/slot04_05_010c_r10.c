/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3094_slot04_05(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
