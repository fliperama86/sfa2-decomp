/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ca400_slot04_16[];
extern u8 data_801ca430_slot04_16[];
extern u16 data_801ca438_slot04_16[];
extern Slot04_16Reca444 data_801ca444_slot04_16[];
extern s16 data_801ca474_slot04_16[];
extern ObjectFnInt data_801ca47c_slot04_16[];
extern u8 data_801ca4ac_slot04_16[];
extern u8 data_801ca514_slot04_16[];
extern SequenceStep *data_801ca550_slot04_16[];
extern ObjectFn data_801ca55c_slot04_16[];
extern ObjectFn data_801ca56c_slot04_16[];
extern ObjectRef data_80190468;

int func_80130184(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_80145f98(Object *object);
void func_80146960(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_8011ffdc(Object *o);
void func_801b5af4_slot04_16(Object *obj);
void func_801b5e08_slot04_16(Object *obj);
void func_801b10b0_slot04_16(Object *obj);
void func_801b10d0_slot04_16(Object *obj);

void func_801b57a0_slot04_16(Object *o) {
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

void func_801b58c4_slot04_16(Object *obj) {
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

void func_801b5938_slot04_16(Object *obj) {
    data_801ca400_slot04_16[obj->field_07](obj);
}

void func_801b5978_slot04_16(Object *obj) {
    ((Slot04bObj *)obj)->field_1a8 = 0;
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38((GameState *)game_state.config, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x29);
}

void func_801b59d4_slot04_16(Object *obj) {
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

void func_801b5a50_slot04_16(Object *obj) {
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
        func_801b5af4_slot04_16(obj);
    }
}

void func_801b5af4_slot04_16(Object *obj) {
    s16 t = *(u16 *)(data_801ca430_slot04_16 + (obj->field_12a & 0xfe));

    if ((u8)func_8013f8c4(obj, -0x26, (s16)(t - 0x26)) == 0) {
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

void func_801b5bc8_slot04_16(Object *o) {
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
            func_801b5e08_slot04_16(o);
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
        func_80140cd8(o, (s16)(*(u16 *)((u8 *)data_801ca438_slot04_16 + ((((Slot04bObj *)o)->field_1a8 + o->field_12a) << 1)) | m), 0);
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

void func_801b5e08_slot04_16(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[4];

    obj->field_50 = data_801ca444_slot04_16[((Slot04bObj *)obj)->field_1a8].a;
    obj->field_58 = data_801ca444_slot04_16[((Slot04bObj *)obj)->field_1a8].b;
    obj->field_f4 = data_801ca444_slot04_16[((Slot04bObj *)obj)->field_1a8].c;
    obj->field_4c = 0;
    obj->field_54 = 0;
}

void func_801b5e80_slot04_16(Object *obj) {
    func_80130184(obj);
    if (obj->field_50 >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_58 = obj->field_f4;
        func_80130efc(obj);
    }
}

void func_801b5ed4_slot04_16(Object *obj) {
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

void func_801b5f9c_slot04_16(Object *obj) {
    u16 t = obj->field_3a;

    if ((t & 0xff) == 0) {
        func_80130efc(obj);
    } else if ((t & 0xff) == 0xf) {
        obj->field_3a = t & 0xff00;
        data_80190468.p->field_63 = obj->field_12a * 4 + 0x3c;
        func_80146960(obj);
        func_80130efc(obj);
        func_801204f4(obj, obj->side, 6);
        if ((u8)func_80140cd8(obj, *(s16 *)((u8 *)data_801ca474_slot04_16 + (obj->field_12a & 0xfe)), 0) != 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        }
        func_80120554(obj, obj->side, 0x319);
    } else {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->field_260 = 1;
        func_80140770(obj, 0x23, 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
        }
        obj->field_4c = 0x38000;
        obj->field_50 = 0x60000;
        obj->field_58 = -0x8000;
        obj->field_54 = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
        }
    }
}

void func_801b6180_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 & 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x2f);
    }
}

void func_801b61fc_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->field_0b = obj->field_0b ^ 1;
        func_801b10d0_slot04_16(obj);
    }
}

void func_801b6268_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b62a8_slot04_16(Object *obj) {
    s16 t = obj->field_3a;

    if (t >= 0) {
        if ((t & 0xff) == 0xf) {
            func_80140cd8(obj, -0xed, 0);
            obj->field_3a = obj->field_3a & 0xff00;
            data_80190468.p->field_63 = obj->field_12a * 4 + 0x3c;
            func_80120554(obj, obj->side, 0x319);
            func_80146960(obj);
        }
        func_80130efc(obj);
    } else {
        obj->field_07 = 5;
        obj->field_45 = 1;
        ((Slot04bObj *)obj)->field_298 = 0;
        ((Slot04bObj *)obj)->field_1a8 = 2;
        func_801b5e08_slot04_16(obj);
        func_801307e0(obj, 0x2c);
    }
}

void func_801b6370_slot04_16(Object *obj) {
    data_801ad398 = data_801ca47c_slot04_16[obj->field_15a](obj);
}

int func_801b63b8_slot04_16(Object *obj) {
    return obj->field_177 != 0;
}

int func_801b63c4_slot04_16(Object *obj) {
    return *(s16 *)&obj->field_c6 >= 0x30;
}

int func_801b63d8_slot04_16(Object *obj) {
    return *(s16 *)&obj->field_c6 >= 0x30;
}

int func_801b63ec_slot04_16(Object *obj) {
    return 1;
}

int func_801b63f4_slot04_16(Object *obj) {
    return 1;
}

int func_801b63fc_slot04_16(Object *obj) {
    return 1;
}

int func_801b6404_slot04_16(Object *obj) {
    return 1;
}

int func_801b640c_slot04_16(Object *obj) {
    return 1;
}

int func_801b6414_slot04_16(Object *obj) {
    return 1;
}

int func_801b641c_slot04_16(Object *obj) {
    return 1;
}

int func_801b6424_slot04_16(Object *obj) {
    return 1;
}

int func_801b642c_slot04_16(Object *obj) {
    return 1;
}

void func_801b6434_slot04_16(Object *obj) {
    s16 i;
    u16 j;

    for (i = 0; i < 7; i++) {
        ref_first.p = (Object *)((u8 *)obj + 0x2b0 + (i << 3));
        for (j = 0; j < 8; j++) {
            *(*(u8 **)&ref_first.p)++ = 0;
        }
    }
}

void func_801b649c_slot04_16(Object *obj) {
    data_801ca55c_slot04_16[obj->field_04](obj);
}

void func_801b64dc_slot04_16(Object *obj) {
    u8 t;

    obj->field_a0 = 0xff;
    obj->field_04 = obj->field_04 + 1;
    obj->field_09 = 0;
    ref_other.p = obj->field_3c;
    t = ref_other.p->field_49;
    ((Slot04bObj *)obj)->field_6c = data_801ca514_slot04_16;
    ((Slot04bObj *)obj)->field_8c = (s32)data_801ca4ac_slot04_16;
    obj->field_45 = 0;
    ((Slot04bObj *)obj)->field_5c = 0;
    obj->field_49 = t;
    func_80130700(obj, data_801ca550_slot04_16[obj->field_ac >> 1]);
}

void func_801b656c_slot04_16(Object *obj) {
    data_801ca56c_slot04_16[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801b65c0_slot04_16(Object *obj) {
    ref_other.p = obj->field_3c;
    if (ref_other.p->frame->field_09 == 0) {
        obj->field_04 = obj->field_04 + 1;
    }
}
