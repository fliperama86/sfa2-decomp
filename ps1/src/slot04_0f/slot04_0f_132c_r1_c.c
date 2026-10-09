/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142ba0(Object *object);

extern ObjectFnInt data_801c5a30_slot04_0f[];
extern ObjectFn data_801c5a60_slot04_0f[];
extern ObjectFn data_801c5a90_slot04_0f[];

void func_801b187c_slot04_0f(Object *obj);
void func_801b16b4_slot04_0f(Object *obj);
int func_801b1c74_slot04_0f(Object *obj);

void func_801b16b4_slot04_0f(Object *obj) {
    u8 t = 1;

    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_15a = 9;
    obj->field_27b = 0x1d;
    obj->field_0b = t;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x19;
    func_801307e0(obj, 0x48);
}

int func_801b171c_slot04_0f(Object *o) {
    Slot *p = &o->slots[16];
    int ok;

    if (o->field_45 != 0) {
        if (p->field_04 != 0) return 0;
        ok = func_801418bc(o);
    } else {
        ok = func_801417cc(o);
    }
    if (ok) {
        o->field_04 = 1;
        o->field_05 = 0;
        o->field_06 = 7;
        o->field_07 = 0;
        o->field_15a = 0;
        o->field_48 = o->field_0b = o->field_158;
        p[1].field_00 = 0;
        func_801b187c_slot04_0f(o);
        return 1;
    }
    return 0;
}

int func_801b17c8_slot04_0f(Object *o) {
    Slot *p = &o->slots[16];
    int ok;
    u8 t;

    if (o->field_45 != 0) {
        if (p->field_04 != 0) return 0;
        ok = func_801418bc(o);
    } else {
        ok = func_801417cc(o);
    }
    if (ok) {
        o->field_04 = 1;
        o->field_06 = 7;
        o->field_05 = 0;
        o->field_07 = 0;
        o->field_15a = 0;
        t = o->field_158;
        o->field_0b = t;
        o->field_48 = t ^ 1;
        p[1].field_00 = 4;
        func_801b187c_slot04_0f(o);
        return 1;
    }
    return 0;
}

void func_801b187c_slot04_0f(Object *obj) {
    u16 t;

    t = obj->field_134 | obj->field_136;
    obj->field_12a = 0;
    obj->field_159 = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->slots[17].field_00 = obj->slots[17].field_00 + 2;
    }
}

int func_801b18c0_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->field_240 != 0) return 0;
    if (!func_801417cc(obj)) return 0;
    one = 1;
    obj->field_04 = one;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = one;
    obj->field_129 = 0;
    obj->field_159 = one;
    t = obj->field_158;
    obj->field_0b = t;
    obj->field_48 = t;
    func_80142b3c(obj);
    return 1;
}

int func_801b1958_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->field_240 != 0) return 0;
    if (!func_801417cc(obj)) return 0;
    one = 1;
    obj->field_04 = one;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 2;
    obj->field_129 = 0;
    obj->field_159 = one;
    t = obj->field_158;
    obj->field_0b = t;
    obj->field_48 = t;
    func_80142b3c(obj);
    return 1;
}

int func_801b19f4_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->field_240 != 0) return 0;
    if (!func_801417cc(obj)) return 0;
    one = 1;
    obj->field_04 = one;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 3;
    obj->field_129 = 0;
    obj->field_159 = one;
    t = obj->field_158;
    obj->field_0b = t;
    obj->field_48 = t;
    func_80142ba0(obj);
    return 1;
}

void func_801b1a90_slot04_0f(Object *obj) {
    data_801ad398 = data_801c5a30_slot04_0f[obj->field_15a](obj);
}

int func_801b1ad8_slot04_0f(Object *obj) {
    u8 t;

    t = obj->field_158;
    obj->slots[17].field_00 = 0;
    obj->field_0b = t;
    obj->field_48 = t;
    if (obj->field_21a != 0) {
        obj->slots[17].field_00 = 4;
    }
    if (obj->field_129 != 0) {
        obj->slots[17].field_00 = obj->slots[17].field_00 + 2;
    }
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1b38_slot04_0f(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_240 != 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1b7c_slot04_0f(Object *obj) {
    if (obj->field_240 != 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1bac_slot04_0f(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1be0_slot04_0f(Object *obj) {
    if (obj->field_177 == 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1c10_slot04_0f(Object *obj) {
    if (*(s32 *)&obj->field_04 != 0x2020202) return 0;
    if (obj->field_45 != 0) return 0;
    if (obj->field_163 != 0) return 0;
    if ((s16)obj->field_5c < 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1c74_slot04_0f(Object *obj) {
    return 1;
}

int func_801b1c7c_slot04_0f(Object *obj) {
    return 0;
}

void func_801b1c84_slot04_0f(Object *obj) {
    data_801c5a60_slot04_0f[obj->field_15a](obj);
}

void func_801b1cc4_slot04_0f(Object *obj) {
    data_801c5a90_slot04_0f[obj->field_15a](obj);
}
