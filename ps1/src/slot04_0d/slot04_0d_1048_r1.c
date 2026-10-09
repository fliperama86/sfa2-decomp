/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b1360_slot04_0d(Object *obj);

extern ObjectFnInt data_801c2980_slot04_0d[];
extern ObjectFn data_801c29a8_slot04_0d[];
extern ObjectFn data_801c29d0_slot04_0d[];
extern ObjectFn data_801c29f8_slot04_0d[];
extern ObjectFn data_801c2a08_slot04_0d[];
extern u8 data_801c2764_slot04_0d[];

int func_801b10cc_slot04_0d(Object *obj) {
    if (obj->field_7e != 0 || obj->field_177 != 0) {
        if (func_801417cc(obj) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 7;
            obj->field_07 = 0;
            obj->field_15a = 9;
            obj->field_159 = 1;
            obj->field_0b = obj->field_158;
            return 1;
        }
    }
    return 0;
}

void func_801b1158_slot04_0d(Object *obj) {
    data_801ad398 = data_801c2980_slot04_0d[obj->field_15a](obj);
}

int func_801b11a0_slot04_0d(Object *obj) {
    return obj->field_14c == 0;
}

int func_801b11ac_slot04_0d(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b11c0_slot04_0d(Object *obj) {
    return 1;
}

int func_801b11c8_slot04_0d(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b11d4_slot04_0d(Object *obj) {
    data_801c29a8_slot04_0d[obj->field_15a](obj);
}

void func_801b1214_slot04_0d(Object *obj) {
    data_801c29d0_slot04_0d[obj->field_15a](obj);
}

void func_801b1254_slot04_0d(Object *obj) {
    data_801c29f8_slot04_0d[obj->field_07](obj);
}

void func_801b1294_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_80130efc(o);
    if ((s16)o->field_3a >= 0) {
        if (o->field_12a != 0) {
            obj->field_1ce = 1;
        }
    } else {
        if (obj->field_1ce == 0 || (obj->field_1ce = 0, obj->field_1cf == 2)) {
            func_801b1360_slot04_0d(o);
        } else {
            obj->field_1cf = obj->field_1cf + 1;
            o->field_12a = o->field_12a - 1;
            func_80141f28(o, 1);
            func_801307e0(o, (u8)(data_801c2764_slot04_0d[obj->field_1cf] + 0x28));
        }
    }
}

void func_801b1360_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 t;

    o->field_07++;
    t = data_801c2764_slot04_0d[obj->field_1cf];
    o->field_225 = 1;
    o->field_12a = t * 2;
    func_801307e0(o, (u8)(t + 0x2b));
}

void func_801b13b4_slot04_0d(Object *obj) {
    data_801c2a08_slot04_0d[obj->field_07](obj);
}

void func_801b13f4_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 4);
    func_80138ae8(&game_state, o);
    obj->field_1ce = 0;
    obj->field_1cf = 0;
    func_801307e0(o, 0x27);
}
