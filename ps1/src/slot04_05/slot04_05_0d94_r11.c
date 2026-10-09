/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b364c_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = (u16)obj->field_70;
    }
    if (*(u8 *)&obj->field_3a != 2) {
        func_80130efc(obj);
    }
}
