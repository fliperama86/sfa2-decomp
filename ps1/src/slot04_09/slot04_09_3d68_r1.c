/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3d68_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_159 = 0;
        obj->field_07 = 0;
        obj->field_a2 = 0;
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}
