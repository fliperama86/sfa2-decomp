/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2a28_slot04_0d[];
extern ObjectFn data_801c2a38_slot04_0d[];
extern ObjectFn data_801c2a40_slot04_0d[];
extern s32 data_801c2774_slot04_0d[];

void func_80146998(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142adc(Object *object);
void func_801b3b6c_slot04_0d(Object *object);
void func_801b19f8_slot04_0d(Object *object);
void func_801b2018_slot04_0d(Object *object);

void func_801b182c_slot04_0d(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 == 0) {
            a = 1;
            b = 0x49;
        } else {
            a = -5;
            b = 0x44;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3b6c_slot04_0d(obj);
    }
    func_80130efc(obj);
}

void func_801b18d0_slot04_0d(Object *obj) {
    int a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_46 = (u8)obj->field_46 | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b195c_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    func_80130efc(o);
    o->field_46 = t = (o->field_46 & 0xff00) | ((o->field_46 & 0xff) - 1);
    if ((t & 0xff) != 0) {
        func_801b19f8_slot04_0d(o);
    } else {
        if (o->field_45 != 0) {
            o->field_04 = 1;
            o->field_05 = 0;
            o->field_06 = 3;
            o->field_07 = 1;
        }
        func_80142c70(o);
    }
}

void func_801b19f8_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = obj->field_27c;
        }
    } else if (*(u8 *)&o->field_46 != 0) {
        (*(u8 *)&o->field_46)--;
    }
    func_80142fe8(o);
}

void func_801b1a64_slot04_0d(Object *obj) {
    data_801c2a28_slot04_0d[obj->field_07](obj);
}

void func_801b1aa4_slot04_0d(Object *obj) {
    s16 t = obj->field_3a;

    if (t >= 0) {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_801307e0(obj, 0x2f);
    }
}

void func_801b1b14_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_80130efc(o);
    if (obj->field_3a != 0) {
        o->field_45 = 1;
        o->field_07++;
        o->field_4c = 0x18000;
        o->field_50 = 0xfff88000;
        o->field_58 = 0x6000;
    }
}

void func_801b1b7c_slot04_0d(Object *obj) {
    int d;

    func_801b2018_slot04_0d(obj);
    if (obj->field_70 > obj->pos_y) {
        d = obj->field_4c;
        if (obj->field_0b == 0) {
            d = -d;
        }
        *(s32 *)&obj->field_10 = d + *(s32 *)&obj->field_10;
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x26);
    }
}

void func_801b1c18_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1c5c_slot04_0d(Object *obj) {
    data_801c2a38_slot04_0d[obj->field_07](obj);
}

void func_801b1c9c_slot04_0d(Object *obj) {
    s16 t = obj->field_3a;

    if (t >= 0) {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_801307e0(obj, 0x30);
    }
}

void func_801b1d0c_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b1d4c_slot04_0d(Object *obj) {
    data_801c2a40_slot04_0d[obj->field_07](obj);
}

void func_801b1d8c_slot04_0d(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    if (obj->field_0b != 0) {
        obj->field_4c = 0x60000;
    } else {
        obj->field_4c = -0x60000;
    }
    a = 0x23;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

void func_801b1e20_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 *t;
    s32 *p;
    u8 g;

    t = data_801c2774_slot04_0d;
    func_80130efc(o);
    g = obj->field_3a;
    if (g & 0x80) {
        *(s32 *)&o->field_10 = *(s32 *)&o->field_10 + o->field_4c;
    } else if (g != 0) {
        o->field_45 = 1;
        o->field_3a = o->field_3a & 0xff00;
        o->field_07++;
        p = t + o->field_12a * 2;
        o->field_4c = p[0];
        o->field_50 = p[1];
        o->field_54 = p[2];
        o->field_58 = p[3];
        o->field_50 = -o->field_50;
        o->field_58 = -o->field_58;
    }
}

void func_801b1ef8_slot04_0d(Object *obj) {
    int d;

    func_801b2018_slot04_0d(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->field_07++;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x26);
    } else {
        if (obj->field_0b != 0) {
            d = obj->field_4c;
        } else {
            d = -obj->field_4c;
        }
        *(s32 *)&obj->field_10 = d + *(s32 *)&obj->field_10;
        obj->field_4c = obj->field_4c + obj->field_54;
        func_80130efc(obj);
    }
}

void func_801b1fbc_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}
