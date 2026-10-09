/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c61ac_slot04_0e[];
extern ObjectFn data_801c61b8_slot04_0e[];
extern u16 data_801c5d84_slot04_0e[];
extern u16 data_801a2804[];
extern u16 box_margin;

Block172 *func_8011f1e0(void);
void func_80141e5c(Object *object);
void func_80142adc(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b4914_slot04_0e(Object *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);
Dir func_801b6b64_slot04_0e(Slot04bObj *obj);
void func_801b6374_slot04_0e(Object *obj);
void func_801b650c_slot04_0e(Object *obj);
void func_801b6698_slot04_0e(Object *obj, u8 a, int b, int c);
void func_801b6808_slot04_0e(Object *obj);
void func_801b687c_slot04_0e(Object *obj);
u8 func_801b6af0_slot04_0e(Object *obj);

void func_801b6168_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 1;
        obj->field_07++;
        func_801307e0(obj, 0x2a);
    }
}

void func_801b61b8_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->field_70 >= obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = 7;
        obj->field_45 = 0;
        obj->field_10 = 0;
        obj->field_14 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x2b);
    }
}

void func_801b6238_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b628c_slot04_0e(Object *obj) {
    Dir buf;
    u16 t;

    if (obj->field_cd != 0) {
        if (func_801b6bd4_slot04_0e(obj) != 0) {
            buf = func_801b6b64_slot04_0e((Slot04bObj *)obj);
            obj->field_48 = buf.first;
            ((Slot04bObj *)obj)->field_1de = 1;
        }
    } else {
        t = obj->field_134;
        if (t & 0x95) {
            buf = func_801b6b64_slot04_0e((Slot04bObj *)obj);
            t = obj->field_134;
            obj->field_48 = buf.first;
            if ((t & 0x94) != 0x94) {
                if ((t & 1) == 0) {
                    ((Slot04bObj *)obj)->field_1de = 1;
                }
            }
        }
    }
}

void func_801b6334_slot04_0e(Object *obj) {
    if (obj->field_129 != 0) {
        func_801b650c_slot04_0e(obj);
    } else {
        func_801b6374_slot04_0e(obj);
    }
}

void func_801b6374_slot04_0e(Object *obj) {
    data_801c61ac_slot04_0e[obj->field_07](obj);
}

void func_801b63b4_slot04_0e(Object *obj) {
    Object *o;

    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    o = obj->other;
    func_80120554(o, o->side, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if ((u16)(box_margin + 0xc0) < obj->pos_x) {
            goto set;
        }
    } else if (obj->field_c2 & 0x8000) {
set:
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x18);
}

void func_801b645c_slot04_0e(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0x10, 0, 1, 0);
    }
    func_80130efc(obj);
}

void func_801b64c0_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    }
}

void func_801b650c_slot04_0e(Object *obj) {
    data_801c61b8_slot04_0e[obj->field_07](obj);
}

void func_801b654c_slot04_0e(Object *obj) {
    Object *o;

    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    o = obj->other;
    func_80120554(o, o->side, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if ((u16)(box_margin + 0xc0) < obj->pos_x) {
            goto set;
        }
    } else if (obj->field_c2 & 0x8000) {
set:
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x19);
    func_80141e5c(obj);
}

void func_801b65fc_slot04_0e(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        func_80141e5c(obj);
    } else {
        *(u8 *)&obj->field_3a = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->other->field_261 = 1;
        func_801b6698_slot04_0e(obj, 0x53, 0, 0);
        func_801b6698_slot04_0e(obj, 0x54, -0x70000, -0x20000);
        func_801b6698_slot04_0e(obj, 0x55, -0x110000, -0x30000);
    }
}

void func_801b6698_slot04_0e(Object *obj, u8 a, int b, int c) {
    Object *p = (Object *)func_8011f1e0();

    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x3b;
        p->field_03 = 5;
        p->field_09 = 0;
        p->field_02 = 0x14;
        p->field_65 = obj->field_65;
        p->field_7a = 0x60;
        p->field_3c = obj;
        p->field_48 = a;
        p->field_4c = b;
        p->field_50 = c;
        p->field_7c = 0x1e0;
        p->field_0d = obj->field_0d;
        p->field_08 = 0x20;
        p->field_90 = obj->field_90;
        p->field_66 = obj->field_66;
        p->field_98 = obj->field_98;
        p->field_9c = obj->field_9c;
    }
}

void func_801b6778_slot04_0e(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_4c = 0xa0000;
        obj->field_54 = -0x2000;
        obj->field_58 = -0x4000;
        obj->field_50 = 0x28000;
        *(u8 *)&obj->field_3a = 0;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
    }
}

void func_801b6808_slot04_0e(Object *obj) {
    u16 i = 0;

    do {
        data_801a2804[obj->field_0d * 16 + i] = data_801c5d84_slot04_0e[i];
        i++;
    } while (i < 0x10);
    func_80137220(0, 6);
}

void func_801b687c_slot04_0e(Object *obj) {
    func_801378d8(obj);
}

void func_801b689c_slot04_0e(Object *obj) {
    Object *o;

    func_801b4914_slot04_0e(obj);
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        *(u8 *)&obj->field_3a = 0;
        o = obj->other;
        if (o->field_15b != 0) {
            func_801b6808_slot04_0e(obj);
            func_801204f4(obj, obj->side, 7);
        }
    }
}

void func_801b6918_slot04_0e(Object *obj) {
    Object *o;

    func_801b4914_slot04_0e(obj);
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        *(u8 *)&obj->field_3a = 0;
        o = obj->other;
        o->field_261 = 0;
        o->field_10 = 0;
        if (o->field_15b != 0) {
            func_80140770(obj, 0, 0xc, 0x10, 0, 1, 0);
        } else {
            func_80140770(obj, 0, 5, -1, 0, 1, 0);
        }
    }
}

void func_801b69b4_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->field_70 < obj->pos_y) {
        obj->field_45 = 0;
        obj->field_10 = 0;
        obj->field_14 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801b687c_slot04_0e(obj);
        obj->field_38 = 1;
        func_80130efc(obj);
    }
}

void func_801b6a30_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    }
}

u8 func_801b6a7c_slot04_0e(Object *obj) {
    u8 r;

    if (obj->field_129 == 0 || obj->field_12a != 2) {
        r = 0;
    } else if (obj->field_cd != 0) {
        r = func_801b6af0_slot04_0e(obj);
    } else {
        r = (obj->field_130 >> 14) & 1;
    }
    return r;
}

u8 func_801b6af0_slot04_0e(Object *obj) {
    return obj->field_219 != 0;
}

void func_801b6afc_slot04_0e(Object *obj) {
    obj->field_4c = obj->field_4c >> 1;
    func_80141f28(obj, 1);
    func_801307e0(obj, 0x1a);
}

void func_801b6b40_slot04_0e(Object *obj) {
    int i;
    u8 z = 0;
    u8 *p = (u8 *)obj + 0x2b0;

    for (i = 0x4f; i >= 0; i--) {
        *p++ = z;
    }
}
