/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6050_slot04_0e[];

void func_80142adc(Object *object);
void func_80138ae8(GameState *state, Object *object);

void func_801b2f58_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);
u8 func_801b6a7c_slot04_0e(Object *obj);
void func_801b6afc_slot04_0e(Object *obj);
Dir func_801b6b64_slot04_0e(Slot04bObj *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);

void func_801b3284_slot04_0e(Object *obj);
void func_801b31b8_slot04_0e(Object *obj, int arg);
void func_801b3200_slot04_0e(Object *obj);
void func_801b339c_slot04_0e(Object *obj);

void func_801b304c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_1de == 0) {
        func_801b3284_slot04_0e(o);
    }
    func_801b4914_slot04_0e(o);
    func_801b31b8_slot04_0e(o, 3);
    if (obj->field_1de != 0) {
        o->field_07 = o->field_07 + 1;
        o->field_28a = 1;
        func_801b3200_slot04_0e(o);
    } else if (o->field_70 < o->pos_y) {
        func_801b2f58_slot04_0e(o);
    } else {
        func_80130efc(o);
    }
}

void func_801b30fc_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    func_801b31b8_slot04_0e(obj, 5);
    if (obj->field_70 < obj->pos_y) {
        func_801b2f58_slot04_0e(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3160_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->field_28a = 0;
        func_801312b8(obj);
    }
}

void func_801b31b8_slot04_0e(Object *obj, int arg) {
    u16 d = arg;

    if (obj->field_cd == 0 && (obj->field_c2 & 0xa000) != 0) {
        if (obj->field_c2 & 0x8000) {
            d = -d;
        }
        *(u16 *)&obj->pos_x = d + *(u16 *)&obj->pos_x;
    }
}

void func_801b3200_slot04_0e(Object *obj) {
    u16 a;
    u16 h;

    if (func_801b6a7c_slot04_0e(obj) != 0) {
        func_801b6afc_slot04_0e(obj);
    } else {
        h = obj->field_12a >> 1;
        func_80141f28(obj, h);
        a = 0x15;
        if (obj->field_129 == 0) {
            a = 0x12;
        }
        func_801307e0(obj, a += h);
    }
}

void func_801b3284_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Dir buf;
    int k;

    if (o->field_cd != 0) {
        k = func_801b6bd4_slot04_0e(o);
    } else {
        k = obj->field_134;
    }
    if (k != 0) {
        buf = func_801b6b64_slot04_0e((Slot04bObj *)o);
        o->field_12a = buf.first;
        o->field_129 = buf.second;
    }
}

void func_801b32f8_slot04_0e(Object *obj) {
    data_801c6050_slot04_0e[obj->field_07](obj);
}

void func_801b3338_slot04_0e(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 2);
    obj->field_58 -= 0x6000;
    func_80138ae8(&game_state, obj);
    func_801b339c_slot04_0e(obj);
}
