/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c612c_slot04_0e[];
extern u8 data_801c6140_slot04_0e[];
extern ObjectFn data_801c6170_slot04_0e[];

void func_80142fe8(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80131468(Object *object);
void func_80142adc(Object *object);

void func_801b4914_slot04_0e(Object *obj);
void func_801b628c_slot04_0e(Object *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);
void func_801b6d7c_slot04_0e(Slot04bObj *obj);
u8 func_801b5b44_slot04_0e(Object *obj);
void func_801b5e34_slot04_0e(Object *obj);
void func_801b5f28_slot04_0e(Object *obj);
void func_801b5f48_slot04_0e(Object *obj);

void func_801b577c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = obj->field_27c;
        }
    } else if (obj->field_47 != 0) {
        obj->field_47--;
    }
    func_80142fe8(o);
}

void func_801b57e8_slot04_0e(Object *obj) {
    data_801c612c_slot04_0e[obj->field_07](obj);
}

void func_801b5828_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 2);
    func_80138ae8(&game_state, o);
    func_801b6d7c_slot04_0e((Slot04bObj *)o);
    o->field_54 = 0;
    o->field_50 = 0;
    o->field_58 = 0;
    obj->field_47 = 8;
    if (o->field_0b == 0) {
        o->field_4c = 0x50000;
    } else {
        o->field_4c = -0x50000;
    }
    a = 0x63;
    if (o->field_49 == 0) {
        a = 0x55;
    }
    a += o->field_12a >> 1;
    func_801307e0(o, a);
}

void func_801b58d8_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    BytePair t;
    u16 a;

    if (obj->field_3a == 0 && func_801b5b44_slot04_0e(o)) {
        t = func_8013054c(o);
        o->field_48 = t.first;
        o->field_07++;
        func_80141f28(o, 2);
        a = 0x66;
        if (o->field_49 == 0) {
            a = 0x22;
        }
        a += o->field_48 >> 1;
        func_801307e0(o, a);
    } else {
        func_801b4914_slot04_0e(o);
        if ((s16)o->field_3a >= 0) {
            func_80130efc(o);
        } else {
            ((Object *)o->other)->field_249 = 5;
            o->field_17b = 0;
            func_80131468(o);
        }
    }
}

void func_801b59b8_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 *p;

    if (obj->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
        func_801209c4(o);
        p = (s32 *)(data_801c6140_slot04_0e + ((o->field_48 << 3) & 0x3f8));
        o->field_4c = *p++;
        o->field_54 = *p++;
        o->field_50 = *p++;
        o->field_58 = *p;
        if (o->field_0b != 0) {
            o->field_4c = -o->field_4c;
            o->field_54 = -o->field_54;
        }
    }
    func_80130efc(o);
}

void func_801b5a70_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->field_70 >= obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->field_14 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x25);
    }
}

void func_801b5af0_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

u8 func_801b5b44_slot04_0e(Object *obj) {
    u8 r;

    if (obj->field_cd != 0) {
        r = func_801b6bd4_slot04_0e(obj);
    } else {
        r = (obj->field_134 & 0x95) != 0;
    }
    return r;
}

void func_801b5b90_slot04_0e(Object *obj) {
    data_801c6170_slot04_0e[obj->field_07](obj);
}

void func_801b5bd0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    obj->field_1de = 0;
    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 2);
    func_80138ae8(&game_state, o);
    func_801b6d7c_slot04_0e((Slot04bObj *)o);
    a = 0x69;
    if (o->field_49 == 0) {
        a = 0x26;
    }
    func_801307e0(o, a);
}

void func_801b5c48_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a != 0) {
        o->field_45 = 1;
        o->field_4c = 0x50000;
        o->field_54 = -0x1800;
        o->field_58 = -0x5000;
        o->field_07++;
        o->field_50 = 0x48000;
        if (o->field_0b != 0) {
            o->field_4c = -o->field_4c;
            o->field_54 = -o->field_54;
        }
    }
    func_80130efc(o);
}

void func_801b5ccc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    if (obj->field_3a == 0 && obj->field_1de == 0) {
        func_801b628c_slot04_0e(o);
    }
    func_801b4914_slot04_0e(o);
    if (o->field_70 > o->pos_y) {
        func_80130efc(o);
    } else {
        o->field_07++;
        o->field_45 = 0;
        o->field_14 = 0;
        o->field_67 = 0;
        o->pos_y = (u16)o->field_70;
        func_801209c4(o);
        a = 0x6a;
        if (o->field_49 == 0) {
            a = 0x21;
        }
        func_801307e0(o, a);
    }
}

void func_801b5d88_slot04_0e(Object *o) {
    s8 t;

    if (((Slot04bObj *)o)->field_3a == 0 && ((Slot04bObj *)o)->field_1de == 0) {
        func_801b628c_slot04_0e(o);
    }
    if ((s16)o->field_3a >= 0) {
        func_80130efc(o);
    } else {
        t = ((Slot04bObj *)o)->field_1de;
        if (t == 0) {
            func_801b5f28_slot04_0e(o);
        } else if (t & 0x80) {
            func_801b5f48_slot04_0e(o);
        } else {
            func_801b5e34_slot04_0e(o);
        }
    }
}
