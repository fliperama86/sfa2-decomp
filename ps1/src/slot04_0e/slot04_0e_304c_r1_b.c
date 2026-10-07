/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6064_slot04_0e[];
extern ObjectFn data_801c606c_slot04_0e[];
extern ObjectFn data_801c6074_slot04_0e[];

int func_80130184(Object *object);
void func_801428e4(Object *object);
void func_80145d20(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801483a4(Object *object, u16 a, u16 b);

void func_801b3450_slot04_0e(Object *obj) {
    int a;
    int x;
    int y;

    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 1;
        obj->field_50 = 0x50000;
        obj->field_58 = -0x5000;
        obj->field_07++;
        if (obj->field_48 == 1) {
            a = 0x37;
            x = -0x38000;
            y = 0x500;
        } else {
            a = 0x38;
            x = 0x40000;
            y = -0x500;
        }
        if (obj->field_0b != 0) {
            x = -x;
            y = -y;
        }
        obj->field_4c = x;
        obj->field_54 = y;
        func_801307e0(obj, a);
        func_801204f4(obj, obj->side, 9);
    }
}

void func_801b3514_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((u8)func_80130184(o) == 0) {
        o->field_14 = 0;
        o->field_45 = 0;
        o->field_159 = 0;
        o->field_17b = 0;
        o->field_07++;
        o->pos_y = obj->field_70;
        func_801209c4(o);
        func_801307e0(o, 0x39);
    } else {
        func_80130efc(o);
    }
}

void func_801b358c_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b35cc_slot04_0e(Object *obj) {
    data_801c6064_slot04_0e[obj->field_07](obj);
}

void func_801b360c_slot04_0e(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_cd != 0) {
        obj->field_48 = obj->field_21a;
    }
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    a = 0x61;
    if (obj->field_49 == 0) {
        a = 0x3c;
    }
    func_801307e0(obj, a);
}

void func_801b3690_slot04_0e(Object *obj) {
    int a;

    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 3;
        obj->field_07 = 1;
        obj->field_45 = 1;
        obj->field_157 = 0;
        obj->field_17b = 0;
        if (obj->field_48 & 0x80) {
            obj->field_4c = 0x78000;
            obj->field_50 = 0x74000;
            obj->field_54 = -0x500;
            obj->field_58 = -0x5a00;
            a = 0x3f;
        } else {
            obj->field_4c = -0x78000;
            obj->field_50 = 0x70000;
            obj->field_54 = 0x500;
            obj->field_58 = -0x5a00;
            a = 0x3e;
        }
        if (obj->field_0b != 0) {
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
        func_801307e0(obj, a);
    }
}

void func_801b377c_slot04_0e(Object *obj) {
    data_801c606c_slot04_0e[obj->field_07](obj);
}

void func_801b37bc_slot04_0e(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x32);
}

void func_801b3810_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_80130efc(o);
    if (obj->field_3a == 0) {
        o->field_165 = 0;
        func_801312b8(o);
    } else if (obj->field_3a != 2) {
        obj->field_3a = 2;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, 0xe, 0x50);
    }
}

void func_801b38a4_slot04_0e(Object *obj) {
    data_801c6074_slot04_0e[obj->field_07](obj);
}

void func_801b38e4_slot04_0e(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    if (((Slot04bObj *)obj)->field_1da == 0) {
        func_80145d20(obj);
    }
    func_801307e0(obj, 0x33);
}
