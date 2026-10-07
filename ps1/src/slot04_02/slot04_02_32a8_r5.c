/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);
int func_80130184(Object *object);

void func_801b38d0_slot04_02(Object *obj) {
    s16 t = obj->field_46;
    int n;

    if ((t & 0xff00) != 0) {
        t = t - 0x100;
        obj->field_46 = t;
        if ((t & 0xff00) == 0) {
            obj->field_50 = 0x74000;
            obj->field_58 = -0x5000;
            if (obj->field_0b != 0) {
                obj->field_4c = -0x10000;
                obj->field_54 = 0x280;
            } else {
                obj->field_4c = 0x10000;
                obj->field_54 = -0x280;
            }
        } else {
            func_80130efc(obj);
        }
    } else if ((u8)func_80130184(obj) != 0) {
        if ((s16)obj->field_3a < 0) {
            obj->field_159 = 0;
            obj->field_07 = obj->field_07 + 1;
            func_80130678(obj, 0x21);
        } else {
            func_80130efc(obj);
        }
    } else {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        n = 0x39;
        if (obj->field_48 == 1) {
            n = 0x38;
        }
        func_801307e0(obj, n);
    }
}
