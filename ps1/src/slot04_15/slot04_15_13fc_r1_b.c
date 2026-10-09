/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80130678(Object *object, int index);
void func_80146998(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b3b74_slot04_15(Object *object);
void func_801b1548_slot04_15(Object *obj);
void func_801b1a00_slot04_15(Object *object);
void func_801b2020_slot04_15(Object *object);

extern u8 data_801c2df4_slot04_15[];
extern ObjectFn data_801c309c_slot04_15[];
extern ObjectFn data_801c30ac_slot04_15[];
extern ObjectFn data_801c30bc_slot04_15[];
extern ObjectFn data_801c30c4_slot04_15[];

void func_801b1548_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;
    int a = 0x2b;

    o->field_07++;
    t = data_801c2df4_slot04_15[obj->field_1cf];
    o->field_225 = 1;
    o->field_12a = t * 2;
    if (o->field_49 != 0) {
        a = 0x49;
    }
    a += t;
    func_801307e0(o, (u8)a);
}

void func_801b15ac_slot04_15(Object *obj) {
    Object *p;
    s16 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        ((Slot04bObj *)obj)->field_46 = 5;
        obj->field_07++;
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x1f;
            p->field_03 = 0;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->field_5c = 0;
            p->pos_y = y;
            p->field_3c = obj;
            obj->field_14c = (s32)p;
            obj->field_240++;
            if (obj->field_12a == 0) {
                func_801204f4(obj, obj->side, 0x13);
            } else {
                func_801204f4(obj, obj->side, 0x14);
            }
        }
    }
}

void func_801b1700_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->field_3a < 0) {
        func_801312b8(o);
    } else {
        if (obj->field_46 != 0) {
            obj->field_46--;
            if (obj->field_46 != 0) {
                goto tail;
            }
            o->field_17b = 0;
        }
        func_80142adc(o);
    tail:
        func_80130efc(o);
    }
}

void func_801b1788_slot04_15(Object *obj) {
    data_801c309c_slot04_15[obj->field_07](obj);
}

void func_801b17c8_slot04_15(Object *obj) {
    int a;

    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    a = 0x2f;
    if (obj->field_45 == 0) {
        a = 0x2b;
    }
    func_80130678(obj, a);
}

void func_801b1834_slot04_15(Object *obj) {
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
        func_801b3b74_slot04_15(obj);
    }
    func_80130efc(obj);
}

void func_801b18d8_slot04_15(Object *obj) {
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

void func_801b1964_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    func_80130efc(o);
    o->field_46 = t = (o->field_46 & 0xff00) | ((o->field_46 & 0xff) - 1);
    if ((t & 0xff) != 0) {
        func_801b1a00_slot04_15(o);
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

void func_801b1a00_slot04_15(Object *o) {
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

void func_801b1a6c_slot04_15(Object *obj) {
    data_801c30ac_slot04_15[obj->field_07](obj);
}

void func_801b1aac_slot04_15(Object *obj) {
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

void func_801b1b1c_slot04_15(Object *o) {
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

void func_801b1b84_slot04_15(Object *obj) {
    int d;

    func_801b2020_slot04_15(obj);
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

void func_801b1c20_slot04_15(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1c64_slot04_15(Object *obj) {
    data_801c30bc_slot04_15[obj->field_07](obj);
}

void func_801b1ca4_slot04_15(Object *obj) {
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

void func_801b1d14_slot04_15(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b1d54_slot04_15(Object *obj) {
    data_801c30c4_slot04_15[obj->field_07](obj);
}
