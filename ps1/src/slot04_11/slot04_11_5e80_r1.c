/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7a2c_slot04_11[];

void func_80130dc0(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);

void func_801b5e80_slot04_11(Object *obj) {
    s16 n;
    s16 t;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, *(s16 *)((u8 *)data_801c7a2c_slot04_11 + (obj->field_12a & 0xfe)));
    n = 0xc;
    if (obj->field_48 != 0) {
        n = 0x12;
    }
    if (obj->field_129 != 0) {
        n += 3;
    }
    t = obj->field_12a >> 1;
    t += n;
    func_801307e0(obj, t);
    if (obj->field_129 != 0 && obj->field_12a == 4) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 5;
        obj->field_07 = 0;
        obj->field_67 = 0;
    }
}

void func_801b5f64_slot04_11(Object *obj) {
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b5f88_slot04_11(Object *obj) {
    u16 t = obj->field_3a;
    s16 d = t & 0x7f;
    s16 e = d;

    if (d != 0) {
        obj->field_3a = t & 0xff80;
        if (obj->field_0b == 0) {
            e = -d;
        }
        obj->pos_x = e + obj->pos_x;
    }
}
