/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6c6c_slot04_10[];
extern u8 data_801c6c9c_slot04_10[];
extern u16 data_801c6ca4_slot04_10[];
extern Slot04_10Rec6cb0 data_801c6cb0_slot04_10[];

int func_80130184(Object *object);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
u8 func_80140cd8(Object *object, int a, int b);
u8 func_8013f8c4(Object *object, int a, int b);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_80145f98(Object *object);
void func_80146960(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801b5abc_slot04_10(Object *obj);
void func_801b5dd0_slot04_10(Object *obj);

void func_801b5768_slot04_10(Object *o) {
    if (*(u8 *)&o->field_3a == 0) {
        func_80130efc(o);
    } else {
        o->field_45 = 1;
        o->field_07++;
        o->field_3a = o->field_3a & 0xff00;
        ref_other.p = o->other;
        ref_other.p->field_15b = 1;
        ref_other.p->field_260 = 1;
        func_80140770(o, 0x22, 0xf, -0x200, 0, 0, 0);
        ref_other.p = o->other;
        if ((s16)ref_other.p->field_5c < 0) {
            o->field_167 = (o->field_12a >> 1) + 0xc;
        }
        o->field_4c = 0x48000;
        o->field_50 = 0x80000;
        o->field_58 = 0xffff7800;
        o->field_54 = 0;
        if (o->field_0b != 0) {
            o->field_4c = -o->field_4c;
        }
    }
}

void func_801b588c_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07 = 8;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x33);
    }
}

void func_801b5900_slot04_10(Object *obj) {
    data_801c6c6c_slot04_10[obj->field_07](obj);
}

void func_801b5940_slot04_10(Object *obj) {
    ((Slot04bObj *)obj)->field_1a8 = 0;
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38((GameState *)game_state.config, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x29);
}

void func_801b599c_slot04_10(Object *obj) {
    int t = -1;

    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            t = 1;
        }
        obj->field_165 = t;
        func_801483a4(obj, -0x13, 0x53);
        func_80120554(obj, obj->side, 0x31c);
    }
    func_80130efc(obj);
}

void func_801b5a18_slot04_10(Object *obj) {
    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        obj->field_07++;
        obj->field_165 = 0;
        obj->field_27b = 0;
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 0xa;
        }
        ref_other.p = obj->other;
        ref_other.p->field_249 = 0;
        func_801307e0(obj, 0x3f);
        func_801b5abc_slot04_10(obj);
    }
}

void func_801b5abc_slot04_10(Object *obj) {
    s16 t = *(u16 *)(data_801c6c9c_slot04_10 + (obj->field_12a & 0xfe));

    if (func_8013f8c4(obj, -0x26, (s16)(t - 0x26)) == 0) {
        obj->field_07 = 0xa;
        func_80130efc(obj);
    } else {
        obj->field_07++;
        if (obj->field_12a != 0) {
            func_801204f4(obj, obj->side, 8);
        } else {
            func_801204f4(obj, obj->side, 4);
        }
        func_80120554(obj, obj->side ^ 1, 0x31a);
        func_80145f98(obj);
        func_801307e0(obj, 0x2a);
    }
}

void func_801b5b90_slot04_10(Object *o) {
    s16 t;
    u16 x;
    int one;
    int m;
    int d;

    func_80130efc(o);
    t = o->field_3a;
    if (t < 0) {
        if (o->field_12a != 0 && ((Slot04bObj *)o)->field_1a8 == 0) {
            ((Slot04bObj *)o)->field_1a8 = 1;
            func_801307e0(o, 0x2b);
        } else {
            one = 1;
            o->field_45 = one;
            ((Slot04bObj *)o)->field_298 = 0;
            o->field_07++;
            func_801307e0(o, 0x2c);
            ((Slot04bObj *)o)->field_1a8 = 0;
            if (o->field_12a == 4) {
                ((Slot04bObj *)o)->field_1a8 = one;
            }
            func_801b5dd0_slot04_10(o);
        }
        return;
    }
    if ((t & 0xff00) != 0) {
        if ((t & 0xff00) != 0x100) {
            o->field_3a = t & 0xff;
            if (o->field_12a == 2) {
                func_801204f4(o, o->side, 4);
            }
        } else {
            o->field_3a = t & 0xff;
            if (o->field_12a == 0) {
                func_801204f4(o, o->side, 5);
            }
        }
    }
    x = o->field_3a;
    if ((x & 0xff) == 0) {
        return;
    }
    if ((x & 0xff) == 1) {
        m = -0x100;
        func_80140cd8(o, (s16)(*(u16 *)((u8 *)data_801c6ca4_slot04_10 + ((((Slot04bObj *)o)->field_1a8 + o->field_12a) << 1)) | m), 0);
        o->field_3a = o->field_3a & m;
        game_state.config->field_63 = 0x3c;
        func_80120554(o, o->side, 0x319);
        if (o->field_12a != 0) {
            if (((Slot04bObj *)o)->field_1a8 == 0) {
                if (o->field_12a != 4) {
                    return;
                }
                func_801204f4(o, o->side, 4);
            } else {
                func_801204f4(o, o->side, 5);
            }
        }
        func_80146960(o);
    } else if ((x & 0xff) == 2) {
        d = 0x58;
        o->field_3a = x & 0xff00;
        if (o->field_0b != 0) {
            d = -0x58;
        }
        o->pos_x = o->pos_x - d;
    } else if ((x & 0xff) == 3) {
        o->field_3a = x & 0xff00;
        if (o->field_12a == 0) {
            func_801307e0(o, 0x2b);
        }
    }
}

void func_801b5dd0_slot04_10(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[4];

    obj->field_50 = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->field_1a8].a;
    obj->field_58 = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->field_1a8].b;
    obj->field_f4 = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->field_1a8].c;
    obj->field_4c = 0;
    obj->field_54 = 0;
}

void func_801b5e48_slot04_10(Object *obj) {
    func_80130184(obj);
    if (obj->field_50 >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_58 = obj->field_f4;
        func_80130efc(obj);
    }
}

void func_801b5e9c_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else if (obj->field_12a == 4 && ((Slot04bObj *)obj)->field_1a8 != 2) {
        obj->field_07 = 0xb;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff00;
        func_801307e0(obj, 0x2d);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801307e0(obj, 0x2e);
    }
}
