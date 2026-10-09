/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2ac8_slot04_0d[];
extern ObjectFn data_801c2adc_slot04_0d[];

void func_80140fe0(Object *object);
void func_80141e5c(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
u8 func_80140cd8(Object *object, int a, int b);
int func_801410c8(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80130678(Object *object, int arg);
void func_80131468(Object *object);
void func_801b30c0_slot04_0d(Object *obj, s16 a);
void func_801b3034_slot04_0d(Object *obj);
void func_801b36c4_slot04_0d(Object *obj);

void func_801b2f4c_slot04_0d(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;
    u16 t;
    int k;

    func_80140fe0(obj);
    func_80130efc(obj);
    func_80141e5c(obj);
    t = obj->field_3a;
    if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        obj->field_45 = 1;
        func_801204f4(obj, obj->side, 0x15);
        func_80146478(obj, 1, 0, 0x4e);
        k = 0;
        if (o->field_1cd == 0) {
            o->field_1cd = 0xff;
            k = 6;
        }
        if (func_80140cd8(obj, k, 0) != 0) {
            func_801204f4(obj, obj->side, 0x16);
            func_801b30c0_slot04_0d(obj, -0x200);
        } else {
            o->field_46 = o->field_46 + 1;
            func_801b3034_slot04_0d(obj);
        }
    } else {
        func_801b3034_slot04_0d(obj);
    }
}

void func_801b3034_slot04_0d(Object *obj) {
    if (*(u8 *)&obj->field_46 != 0) {
        if (obj->other->field_15b == 0) {
            func_801b30c0_slot04_0d(obj, 2);
        } else if ((u8)func_801410c8(obj) != 0) {
            obj->field_07 = obj->field_07 + 1;
            func_801307e0(obj, 0x19);
            func_80141e5c(obj);
        }
    }
}

void func_801b30c0_slot04_0d(Object *obj, s16 a) {
    int x = 0x40000;
    int t;

    obj->field_07 = 6;
    t = obj->field_0b ^ 1;
    obj->field_0b = t;
    if (t != 0) {
        x = -0x40000;
    }
    obj->field_4c = x;
    obj->field_50 = -0x40000;
    obj->field_58 = 0x6000;
    func_80140770(obj, 0x1e, 0xf, a, 0, 0, 0);
    func_80130678(obj, 0x14);
}

void func_801b314c_slot04_0d(Object *obj) {
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

void func_801b31e8_slot04_0d(Object *obj) {
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

void func_801b3284_slot04_0d(Object *obj) {
    func_801b36c4_slot04_0d(obj);
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

void func_801b32fc_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    }
}

void func_801b3348_slot04_0d(Object *obj) {
    s16 y;

    func_801b36c4_slot04_0d(obj);
    if (((Slot04bObj *)obj)->field_52 <= 0 || obj->pos_y < (y = obj->field_70)) {
        func_80130efc(obj);
        func_80130efc(obj);
    } else {
        obj->pos_y = y;
        obj->field_45 = 0;
        func_801312b8(obj);
    }
}

void func_801b33c0_slot04_0d(Object *obj) {
    data_801c2ac8_slot04_0d[obj->field_07](obj);
}

void func_801b3400_slot04_0d(Object *obj) {
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

void func_801b347c_slot04_0d(Object *obj) {
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

void func_801b3590_slot04_0d(Object *obj) {
    func_801b36c4_slot04_0d(obj);
    if (((Slot04bObj *)obj)->field_52 < 0 || obj->pos_y < (s16)(obj->field_70 - 0x50)) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        func_801307e0(obj, 0x21);
    }
}

void func_801b360c_slot04_0d(Object *obj) {
    func_801b36c4_slot04_0d(obj);
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

void func_801b3684_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b36c4_slot04_0d(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
}

void func_801b36f8_slot04_0d(Object *obj) {
    func_801312b8(obj);
}

void func_801b3718_slot04_0d(Object *obj) {
    func_80131468(obj);
}

void func_801b3738_slot04_0d(Object *obj) {
    data_801c2adc_slot04_0d[obj->field_06](obj);
}
