/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c61d4_slot04_0e[];
extern ObjectFn data_801c6214_slot04_0e[];
extern u16 data_801c6254_slot04_0e[];
extern u16 data_801c6294_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

void func_801b6e9c_slot04_0e(Slot04bObj *obj);
void func_801b6f18_slot04_0e(Slot04bObj *obj);

BytePair func_801b6b64_slot04_0e(Slot04bObj *obj) {
    BytePair d;

    obj->field_1de = 0xff;
    if (obj->field_cd == 0) {
        d = func_8013054c((Object *)obj);
    } else {
        d.first = obj->field_1dc;
        d.second = obj->field_1dd;
    }
    return d;
}

u8 func_801b6bd4_slot04_0e(Object *obj) {
    return data_801c61d4_slot04_0e[(obj->field_210 & 0x1e) >> 1](obj);
}

int func_801b6c1c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[obj->field_a6];

    u16 t = rec->field_18;

    t = t - 1;
    rec->field_18 = t;
    return (u32)(t << 16) >> 31;
}

int func_801b6c58_slot04_0e(Object *obj) {
    return 0;
}

int func_801b6c60_slot04_0e(Object *obj) {
    return 1;
}

int func_801b6c68_slot04_0e(Object *obj) {
    return obj->field_50 < 0;
}

int func_801b6c74_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[obj->field_a6];
    s16 x;
    u8 k;
    u16 t = rec->field_18;

    t = t - 1;
    rec->field_18 = t;
    if ((t << 16) >= 0) {
        return 0;
    }
    x = obj->field_21e;
    if (x < 0x58) {
        return 0;
    }
    k = 0;
    if (x >= 0x90) {
        k = 2;
        if (x >= 0xc0) {
            k = 4;
        }
    }
    obj->field_1dc = k;
    return 1;
}

int func_801b6cf0_slot04_0e(Object *o) {
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[o->side];
    u16 t;

    if (((Slot04bObj *)o)->field_1de != 0) {
        return 1;
    }
    t = rec->field_18;
    t = t - 1;
    rec->field_18 = t;
    if ((t << 16) >= 0) {
        return 0;
    }
    ((Slot04bObj *)o)->field_1dc -= 1;
    if ((((Slot04bObj *)o)->field_1dc & 0x80) != 0) {
        return 0;
    }
    func_801b6e9c_slot04_0e((Slot04bObj *)o);
    return 1;
}

void func_801b6d7c_slot04_0e(Slot04bObj *obj) {
    if (obj->field_cd != 0) {
        data_801c6214_slot04_0e[(obj->field_210 & 0x1e) >> 1]((Object *)obj);
    }
}

void func_801b6dd0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    func_801b6e9c_slot04_0e(obj);
    func_801b6f18_slot04_0e(obj);
}

void func_801b6e00_slot04_0e(Object *obj) {
}

void func_801b6e08_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    obj->field_1dc = obj->field_12a;
    obj->field_1dd = 0;
}

void func_801b6e18_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    func_801b6e9c_slot04_0e(obj);
}

void func_801b6e38_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_0eRec62b4 *rec;
    u16 t;

    obj->field_1dc = obj->field_12a >> 1;
    obj->field_1dd = 0;
    rec = &data_801c62b4_slot04_0e[obj->field_a6];
    func_801b6e9c_slot04_0e(obj);
    t = rec->field_18;
    rec->field_18 = t + (t >> 1);
}

void func_801b6e9c_slot04_0e(Slot04bObj *obj) {
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[obj->field_a6];

    rec->field_18 = data_801c6254_slot04_0e[obj->field_cf & 0x1f] + (func_80151184() & 0x1f);
}

void func_801b6f18_slot04_0e(Slot04bObj *obj) {
    *(u16 *)&obj->field_1dc = data_801c6294_slot04_0e[func_80151184() & 0xf];
}
