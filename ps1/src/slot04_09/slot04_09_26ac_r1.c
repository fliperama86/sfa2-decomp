/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);
void func_80142adc(Object *object);

void func_801b26ac_slot04_09(Object *obj) {
    func_801b2858_slot04_09(obj);
    if (obj->field_50 < 0) {
        obj->field_07++;
        obj->field_58 = obj->field_58 * 3;
        obj->field_0b = obj->field_0b ^ 1;
        func_801307e0(obj, 0x4a);
    }
}

void func_801b2714_slot04_09(Object *obj) {
    func_801b2858_slot04_09(obj);
    if (obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->field_14 = 0;
        obj->field_17b = 0;
        obj->pos_y = obj->field_70;
        func_80142adc(obj);
        func_801209c4(obj);
        func_801307e0(obj, 0x4b);
    }
}

void func_801b279c_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        obj->field_17b = 0;
        func_80142adc(obj);
        func_80130efc(obj);
    }
}
