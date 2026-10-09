/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2eac_slot04_15[];
extern u8 data_801c2ecc_slot04_15[];
extern ObjectFn data_801c314c_slot04_15[];
extern ObjectFn data_801c3160_slot04_15[];
extern ObjectFn data_801c3170_slot04_15[];

Block172 *func_8011f1e0(void);
void func_80130678(Object *object, int arg);
void func_80141e5c(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80131468(Object *object);
void func_801b36cc_slot04_15(Object *obj);
void func_801b39d0_slot04_15(Object *object, GameState *g);

void func_801b3154_slot04_15(Object *obj) {
    Object *o;

    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
        func_80141e5c(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        func_80146478(obj, 1, 0x18, 0x47);
        o = obj->other;
        ref_other.p = o;
        func_80120554(o, ref_other.p->side, 0x306);
        func_801307e0(obj, 0x1a);
    }
}

void func_801b31f0_slot04_15(Object *obj) {
    Object *o;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_4c = 0;
        obj->field_50 = 0;
        obj->field_58 = 0x6000;
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0x1e, 0xf, 1, 0, 1, 1);
        o = obj->other;
        if (o->field_15b != 0) {
            o->field_243 = 0;
            func_801204f4(obj, obj->side, 0xb);
        }
    }
}

void func_801b328c_slot04_15(Object *obj) {
    func_801b36cc_slot04_15(obj);
    if (obj->field_70 > obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x1b);
    }
}

void func_801b3304_slot04_15(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    }
}

void func_801b3350_slot04_15(Object *obj) {
    s16 y;

    func_801b36cc_slot04_15(obj);
    if (((Slot04bObj *)obj)->field_52 <= 0 || obj->pos_y < (y = obj->field_70)) {
        func_80130efc(obj);
        func_80130efc(obj);
    } else {
        obj->pos_y = y;
        obj->field_45 = 0;
        func_801312b8(obj);
    }
}

void func_801b33c8_slot04_15(Object *obj) {
    data_801c314c_slot04_15[obj->field_07](obj);
}

void func_801b3408_slot04_15(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_0b = 0;
    if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x20);
    func_80141e5c(obj);
}

void func_801b3484_slot04_15(Object *obj) {
    Object *o;
    int x;

    func_80130efc(obj);
    func_80141e5c(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        func_80146478(obj, 2, -0x49, 0x30);
        x = 0x36000;
        if (obj->field_0b != 0) {
            x = -0x36000;
        }
        obj->field_50 = -0x40000;
        obj->field_4c = x;
        obj->field_58 = 0x6000;
        func_80140770(obj, 0x1d, 5, 0xf, 0, 0, 0);
        o = obj->other;
        ref_other.p = o;
        if ((s16)obj->field_5c >= 0) {
            func_80120554(o, ref_other.p->side, 0x30f);
        } else {
            func_80120554(o, ref_other.p->side, 0x346);
        }
    }
}

void func_801b3598_slot04_15(Object *obj) {
    func_801b36cc_slot04_15(obj);
    if (((Slot04bObj *)obj)->field_52 < 0 || (s16)(obj->field_70 - 0x50) >= obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        func_801307e0(obj, 0x21);
    }
}

void func_801b3614_slot04_15(Object *obj) {
    func_801b36cc_slot04_15(obj);
    if (obj->field_70 > obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x22);
    }
}

void func_801b368c_slot04_15(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b36cc_slot04_15(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
}

void func_801b3700_slot04_15(Object *obj) {
    func_801312b8(obj);
}

void func_801b3720_slot04_15(Object *obj) {
    func_80131468(obj);
}

void func_801b3740_slot04_15(Object *obj) {
    data_801c3160_slot04_15[obj->field_06](obj);
}

void func_801b3780_slot04_15(Object *object) {
    object->field_06 = object->field_06 + 1;
    object->field_0b = object->field_158;
    func_80130678(object, 0);
}

void func_801b37b4_slot04_15(Object *object) {
    if (game_state.field_5c == 0) {
        object->field_06 = object->field_06 + 1;
    }
    func_80130efc(object);
}

void func_801b37f0_slot04_15(Object *object) {
    u16 a;
    ((Slot04bObj *)object)->field_46 = 0x3c;
    object->field_06 = object->field_06 + 1;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    a = object->field_c2 & 0xf000;
    if (((Slot04bObj *)object)->field_5c >= 0x90 && a == 0x1000) {
        a = 1;
    } else {
        a = func_80151184();
        a &= 0x1f;
        if (((Slot04bObj *)object)->field_5c < 0x80) {
            a = data_801c2eac_slot04_15[a];
        } else {
            a = data_801c2ecc_slot04_15[a];
        }
    }
    a = (s8)func_80125734(object, (s8)a); object->field_12c = a; a += 0x23; func_80130678(object, (s16)a);
}

void func_801b38e8_slot04_15(Object *object) {
    data_801c3170_slot04_15[object->field_12c](object);
}

void func_801b3928_slot04_15(Object *object) {
    int a;
    s8 r;

    if ((s16)object->field_3a < 0) {
        r = func_80151184();
        a = r;
        if ((a & 3) != 0) {
            a = 0x32;
        } else {
            a = 0x33;
        }
        func_80130678(object, (u8)a);
    }
    func_801b39d0_slot04_15(object, &game_state);
    func_80130efc(object);
}

void func_801b3998_slot04_15(Object *object) {
    func_801b39d0_slot04_15(object, &game_state);
    func_80130efc(object);
}

void func_801b39d0_slot04_15(Object *object, GameState *g) {
    if (((Slot04bObj *)object)->field_46 != 0) {
        ((Slot04bObj *)object)->field_46--;
        if (((Slot04bObj *)object)->field_46 == 0) {
            g->field_4b |= 1 << object->side;
        }
    }
}

void func_801b3a1c_slot04_15(Object *object) {
    Object *p;
    int dx;
    u16 e;
    u8 d;

    func_80130efc(object);
    if (object->field_3a & 0xff) {
        object->field_3a = object->field_3a & 0xff00;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x77;
            p->field_3c = object;
            p->field_0c = object->field_0c;
            p->field_0d = object->field_0d;
            p->field_0e = object->field_0e;
            p->field_0b = object->field_0b;
            p->field_1c = object->field_1c;
            e = object->field_1e;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_1e = e;
            d = object->field_0d;
            p->field_02 = 0x17;
            p->field_08 = 0x20;
            p->field_0d = d;
            p->field_90 = object->field_90;
            p->field_98 = object->field_98;
            p->field_9c = object->field_9c;
            p->field_66 = object->field_66;
            *(s32 *)&p->field_10 = *(s32 *)&object->field_10;
            *(s32 *)&p->field_14 = *(s32 *)&object->field_14;
            p->pos_y = p->pos_y - 0x63;
            dx = -0x3b;
            if (object->field_0b != 0) {
                dx = 0x3b;
            }
            p->pos_x = dx + p->pos_x;
        }
    }
    func_801b39d0_slot04_15(object, &game_state);
}
