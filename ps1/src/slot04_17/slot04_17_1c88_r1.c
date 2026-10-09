/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801cdfd0_slot04_17[];
extern u8 data_801ce32c_slot04_17[];
extern u8 data_801ce330_slot04_17[];
extern ObjectFn data_801ce338_slot04_17[];
extern s32 data_801ce348_slot04_17[];
extern ObjectFn data_801ce354_slot04_17[];
extern u8 data_801ce378_slot04_17[];
extern u8 data_801ce380_slot04_17[];

void func_80142adc(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b1c48_slot04_17(Object *obj);
void func_801b1f44_slot04_17(Object *obj);
int func_801b5724_slot04_17(Object *obj);
int func_801b56e0_slot04_17(Object *obj);
void func_801b57f0_slot04_17(Object *obj);
void func_801b5810_slot04_17(Object *obj);
Object *func_801b5890_slot04_17(Object *obj);

void func_801b1c88_slot04_17(Object *o) {
    u8 t;

    if (func_801b5724_slot04_17(o) < 0 || ((Slot04bObj *)o)->field_47 == 0) {
        func_801b1c48_slot04_17(o);
    } else {
        ((Slot04bObj *)o)->field_47--;
        ((Slot04bObj *)o)->field_46++;
        if (o->field_67 != 0) {
            if (o->field_cd != 0 ? (((Slot04bObj *)o)->field_47 < 6 &&
                                    (func_8014a170(o, (u32 *)data_801cdfd0_slot04_17) & 0xff) == 0)
                                 : ((o->field_134 & data_801ce32c_slot04_17[o->field_12a >> 1]) != 0)) {
                o->field_12f++;
                t = ((Slot04bObj *)o)->field_46;
                if (t != 0) {
                    t--;
                }
                ((Slot04bObj *)o)->field_47 = t;
                func_801204f4(o, o->side, 6);
                ((Slot04bObj *)o)->field_46 = 0;
                o->field_67 = 0;
                if (o->field_12f == o->field_12e) {
                    o->field_54 = -0x2000;
                    o->field_58 = -0x5000;
                    o->field_07++;
                }
                o->field_4c = 0xc000;
                o->field_50 = 0x19000;
                func_801b1f44_slot04_17(o);
            }
        }
        func_80130efc(o);
    }
}

void func_801b1dec_slot04_17(Object *obj) {
    int a;

    if (func_801b5724_slot04_17(obj) < 0) {
        a = -0x7000;
        obj->field_07++;
        if (obj->field_49 != 0) {
            a = -0x15000;
        }
        obj->field_58 = a;
    }
    func_80130efc(obj);
}

void func_801b1e44_slot04_17(Object *obj) {
    func_801b5724_slot04_17(obj);
    if (obj->pos_y >= obj->field_70) {
        obj->field_07++;
        func_801209c4(obj);
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x3d);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1ed0_slot04_17(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b1f44_slot04_17(Object *obj) {
    u16 t = *(u16 *)(data_801ce330_slot04_17 + (obj->field_12a & 0xfe));

    t = obj->field_12f + t;
    func_801307e0(obj, t);
}

void func_801b1f8c_slot04_17(Object *obj) {
    data_801ce338_slot04_17[obj->field_07](obj);
}

void func_801b1fcc_slot04_17(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_29c = 2;
    obj->field_07++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    obj->field_0b = obj->field_158;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x3a;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}

void func_801b2050_slot04_17(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        obj->field_4c = data_801ce348_slot04_17[obj->field_12a >> 1];
    }
    func_80130efc(obj);
}

void func_801b20b0_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 d = o->field_4c;

    if (o->field_0b == 0) {
        d = -d;
    }
    *(s32 *)&o->field_10 += d;
    if (*(u8 *)&o->field_3a == 0) {
        o->field_54 = -0xc000;
        obj->field_47 = 3;
        o->field_29c = 0;
        o->field_07++;
    }
    func_80130efc(o);
}

void func_801b211c_slot04_17(Object *o) {
    s32 d;
    u16 t;

    if (((Slot04bObj *)o)->field_47 != 0) {
        ((Slot04bObj *)o)->field_47--;
        if (((Slot04bObj *)o)->field_47 == 0) {
            ((Slot04bObj *)o)->field_47 = 3;
            func_80148e84(o);
        }
    }
    if ((o->field_3a << 16) < 0) {
        ref_other.p = o->other;
        ref_other.p->field_249 = 5;
        o->field_17b = 0;
        func_801b5810_slot04_17(o);
    } else {
        d = o->field_4c;
        if (o->field_0b == 0) {
            d = -d;
        }
        *(s32 *)&o->field_10 += d;
        o->field_4c += o->field_54;
        if (o->field_4c < 0) {
            o->field_4c = 0;
            o->field_54 = 0;
            ((Slot04bObj *)o)->field_47 = 0;
        }
        t = o->field_3a;
        if ((t & 0xff) != 0) {
            o->field_3a = t & 0xff00;
            o->field_17b = 0;
        }
        func_80142adc(o);
        func_80130efc(o);
    }
}

void func_801b2228_slot04_17(Object *obj) {
    data_801ce354_slot04_17[obj->field_07](obj);
}

void func_801b2268_slot04_17(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    a = 0x24;
    if (obj->field_49 != 0) {
        a = 0x3d;
    }
    func_801307e0(obj, a);
}

void func_801b22d4_slot04_17(Object *obj) {
    u16 t;
    s32 d;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 0) {
        t = obj->field_130;
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_48 = 0;
        obj->field_67 = 0;
        if (obj->field_cd != 0) {
            t = func_80151184();
        }
        if (t & 0x8000) {
            obj->field_0b ^= 1;
        }
        d = *(s32 *)&func_801b5890_slot04_17(obj)->field_10;
        if (obj->field_0b == 0) {
            d += 0x1800000;
        }
        d -= *(s32 *)&obj->field_10;
        d /= *(u16 *)(data_801ce378_slot04_17 + (obj->field_12a & 0xfe));
        if (obj->field_49 != 0) {
            d <<= 2;
        }
        obj->field_50 = 0xa5000;
        obj->field_4c = d;
        obj->field_54 = 0;
        obj->field_58 = -0x8000;
    }
}

void func_801b23e8_slot04_17(Object *obj) {
    int r = func_801b56e0_slot04_17(obj);

    if (r < 0 && obj->pos_y >= obj->field_70) {
        func_801209c4(obj);
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_07 = 8;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801307e0(obj, 0x37);
    } else if (obj->pos_y < obj->field_70 - 0x48 && (data_801ce380_slot04_17[obj->field_0b] & obj->field_164)) {
        obj->field_07++;
        func_801209c4(obj);
        obj->field_50 = 0;
        obj->field_58 = 0;
        func_801307e0(obj, 0x25);
    } else {
        func_80130efc(obj);
    }
}
