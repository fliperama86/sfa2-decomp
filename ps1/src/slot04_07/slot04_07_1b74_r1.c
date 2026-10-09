/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);
void func_801b1cac_slot04_07(Object *obj);
void func_801b1d5c_slot04_07(Object *obj);
void func_801b1dfc_slot04_07(Object *obj);

void func_801b1b74_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) < 0 && obj->pos_y >= obj->field_70) {
        obj->pos_y = obj->field_70;
        obj->field_07 = 5;
        obj->field_45 = 0;
        obj->field_17b = 0;
        func_801209c4(obj);
        func_801307e0(obj, 0x3b);
    } else {
        if (obj->field_50 >= 0) {
            func_80130efc(obj);
        } else if (obj->field_cd != 0) {
            func_801b1cac_slot04_07(obj);
        } else if (!((obj->field_134 | obj->field_136) & 0x94)) {
            func_80130efc(obj);
        } else if (obj->other->field_164 != 0 || obj->other->field_157 != 0 || (u8)func_8014025c(obj, -8, 0x24, -0x24, 0x18) == 0) {
            func_801b1dfc_slot04_07(obj);
        } else {
            func_801b1d5c_slot04_07(obj);
        }
    }
}
