/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1aa4_slot04_0a(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 1;
        obj->field_67 = 0;
        obj->field_07++;
        func_801307e0(obj, 0x22);
    }
}
