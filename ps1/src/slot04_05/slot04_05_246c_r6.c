/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3048_slot04_05(Object *object);

void func_801b2c84_slot04_05(Object *obj) {
    s16 t = obj->field_3a;
    if (t & 0x8000) {
        obj->field_07++;
        if (obj->field_0b != 0) {
            obj->field_4c = 0x40000;
        } else {
            obj->field_4c = 0xfffc0000;
        }
        obj->field_50 = 0x90000;
        obj->field_58 = -0x7000;
        obj->field_54 = 0;
        obj->field_45 = 1;
        func_801307e0(obj, 0x2e);
    } else {
        switch ((u8)t) {
        case 0:
            break;
        default:
            if (obj->field_4c >= 0) {
                func_801b3048_slot04_05(obj);
            }
            break;
        case 3:
            obj->field_4c = 0x40000;
            obj->field_54 = 0xffff0000;
            break;
        }
        func_80130efc(obj);
    }
}

int func_80130184(Object *object);

void func_801b2d58_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        if ((u8)obj->field_3a != 1) {
            func_80130efc(obj);
        }
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
    }
}
