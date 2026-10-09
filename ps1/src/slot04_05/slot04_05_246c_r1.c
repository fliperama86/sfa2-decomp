/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b246c_slot04_05(Object *obj) {
    if (obj->field_50 < 0) {
        obj->field_58 -= 0x3000;
    }
    if ((u8)func_80130184(obj) != 0) {
        if ((u8)obj->field_3a != 2) {
            func_80130efc(obj);
        }
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
    }
}
