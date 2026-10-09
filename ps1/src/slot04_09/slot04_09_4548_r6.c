/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);

void func_801b5c6c_slot04_09(Object *obj);
void func_801b2858_slot04_09(Object *obj);

void func_801b4c14_slot04_09(Object *obj) {
    s16 t;
    s16 a;
    obj->field_46--;
    func_80130efc(obj);
    func_801b5c6c_slot04_09(obj);
    if (obj->field_3a & 0x80) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
    }
    t = obj->field_3a & 0x7f;
    if (t != 0) {
        if (obj->field_0b != 0) {
            a = t;
        } else {
            a = -t;
        }
        obj->pos_x = a + obj->pos_x;
        obj->field_3a = obj->field_3a & 0xff00;
    }
}

void func_801b4cc0_slot04_09(Object *obj) {
    int a = 0x3c;
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((t << 16) < 0) {
        obj->field_07++;
        if (obj->field_49 != 0) {
            a = 0x4f;
        }
        func_801307e0(obj, (obj->field_12a >> 1) + a);
    } else {
        func_801b2858_slot04_09(obj);
        if (obj->field_4c == 0) {
            obj->field_54 = 0;
        }
        if ((u8)obj->field_3a != 0) {
            obj->field_17b = 0;
            func_80142adc(obj);
        } else {
            func_801b5c6c_slot04_09(obj);
        }
        func_80130efc(obj);
    }
}
