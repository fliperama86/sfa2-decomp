/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

extern ObjectFn data_801ca12c_slot04_16[];
extern ObjectFn data_801ca134_slot04_16[];
extern ObjectFn data_801ca140_slot04_16[];
extern ObjectFn data_801ca154_slot04_16[];
void func_801204f4(Object *o, int b, int c);
void func_80120554(Object *o, int b, unsigned c);
void func_801307e0(Object *object, int arg);
void func_80130efc(Object *object);
void func_801312b8(Object *object);
void func_80141f28(Object *object, short delta);
u8 func_80146840(Object *object);
u8 func_80151184(void);
int func_801418bc(Object *object);
extern u16 box_margin;
extern ObjectRef data_80190468;

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
u8 func_80140cd8(Object *object, int a, int b);
void func_80140fe0(Object *object);
int func_801410c8(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80146960(Object *object);
void func_80130678(Object *object, u16 index);
void func_80131468(Object *object);
void func_80131638(Object *object);

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
u8 func_8013cac8(Object *object, u8 a, u8 b);
u8 func_8013caf0(Object *object, u8 a, u8 b);
u8 func_8013ccb4(Object *object, u8 a, u8 b);
u8 func_8013cd5c(Object *object, u8 a, u8 b);
int func_8013d0c8(Object *object);
int func_8013d0fc(Object *object);
int func_80141788(Object *object);
int func_80141e34(Object *object);

/* functions of other units of this module */
int func_801b235c_slot04_16(Object *obj);
int func_801b2238_slot04_16(Object *obj);
u8 func_801b1d38_slot04_16(Object *obj);
u8 func_801b1dc4_slot04_16(Object *obj);
u8 func_801b1e64_slot04_16(Object *obj);
u8 func_801b1ed0_slot04_16(Object *obj);
u8 func_801b1fc4_slot04_16(Object *obj);
u8 func_801b2034_slot04_16(Object *obj);
u8 func_801b20a4_slot04_16(Object *obj);
u8 func_801b210c_slot04_16(Object *obj);

void func_801b10b0_slot04_16(Object *obj);
void func_801b1340_slot04_16(Object *obj);
void func_801b1700_slot04_16(Object *obj);
u8 func_801b1c50_slot04_16(Object *obj);

void func_801b0fb0_slot04_16(Object *obj) {
    data_801ca12c_slot04_16[obj->field_06](obj);
}

void func_801b0ff0_slot04_16(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    if ((func_80151184() & 1) == 0 && (obj->field_c2 & 0x100) == 0) {
        func_801204f4(obj, obj->side, 0xb);
        func_801307e0(obj, 0x45);
    } else {
        func_80130678(obj, 0x22);
    }
}

void func_801b1070_slot04_16(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
    func_80130efc(obj);
}

void func_801b10b0_slot04_16(Object *obj) {
    func_801312b8(obj);
}

void func_801b10d0_slot04_16(Object *obj) {
    func_80131468(obj);
}

void func_801b10f0_slot04_16(Object *obj) {
    func_80131638(obj);
}

void func_801b1110_slot04_16(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b1700_slot04_16(obj);
    } else if (obj->field_129 != 0) {
        func_801b1340_slot04_16(obj);
    } else {
        data_801ca134_slot04_16[obj->field_07](obj);
    }
}

void func_801b1190_slot04_16(Object *obj) {
    int t;

    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        t = (s16)(box_margin + 0xc0) < obj->pos_x;
    } else {
        t = obj->field_c2 & 0x2000;
    }
    if (t) {
        obj->field_0b = 1;
    }
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x1c);
}

void func_801b1230_slot04_16(Object *obj) {
    u16 a = obj->field_3a;

    if ((a & 0xff) != 2) {
        if ((a & 0xff) != 1) {
            func_80130efc(obj);
        } else {
            obj->field_07 = obj->field_07 + 1;
            func_80140770(obj, 0, 0xf, -0x200, 0, 0, 0);
            func_80130efc(obj);
        }
    } else {
        obj->field_3a = a & 0xff00;
        data_80190468.p->field_63 = 0x3c;
        func_80146960(obj);
        func_80130efc(obj);
        func_80140cd8(obj, 0x12, 0);
        func_80120554(obj, obj->side, 0x319);
    }
}

void func_801b1300_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b1340_slot04_16(Object *obj) {
    data_801ca140_slot04_16[obj->field_07](obj);
}

void func_801b1380_slot04_16(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    obj->field_160 = 0x64;
    obj->field_46 = 0;
    ((Slot04bObj *)obj)->field_1c0 = 0;
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x18);
}

void func_801b13e8_slot04_16(Object *obj) {
    u16 a;
    s16 i;

    ref_other.p = obj->other;
    a = obj->field_3a;
    if ((a & 0xff) == 1) {
        obj->field_3a = a & 0xff00;
        if (func_80146840(obj) != 0) {
            i = -0x4f;
            if (obj->field_0b != 0) {
                i = 0x4f;
            }
            *(u16 *)&ref_other.p->pos_x += i;
            *(u16 *)&ref_other.p->pos_y -= 0x59;
        }
        i = 0;
        if (((Slot04bObj *)obj)->field_1c0 == 0) {
            i = 0xa;
            ((Slot04bObj *)obj)->field_1c0 = ((Slot04bObj *)obj)->field_1c0 + 1;
        }
        if (func_80140cd8(obj, (s16)i, 0) != 0) {
            func_801204f4(obj, obj->side, 0x15);
            obj->field_07 = 3;
            func_801307e0(obj, 0x19);
        } else {
            func_801204f4(obj, obj->side, 0x13);
            obj->field_46 = obj->field_46 + 1;
            func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            func_80130efc(obj);
        } else {
            obj->field_07 = 2;
            func_801307e0(obj, 0x19);
        }
        }
    } else {
        func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            func_80130efc(obj);
        } else {
            obj->field_07 = 2;
            func_801307e0(obj, 0x19);
        }
    }
}

void func_801b15c8_slot04_16(Object *obj) {
    u8 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t == 1) {
        if (((Slot04bObj *)obj)->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, t);
        } else {
            func_80140770(obj, 0, 5, 0xa, 0, 0, t);
        }
        obj->field_07 = 4;
    }
}

void func_801b1644_slot04_16(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a == 1) {
        if (((Slot04bObj *)obj)->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 0);
        } else {
            func_80140770(obj, 0, 5, 0xa, 0, 0, 0);
        }
        obj->field_07 = 4;
    }
}

void func_801b16c0_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b1700_slot04_16(Object *obj) {
    data_801ca154_slot04_16[obj->field_07](obj);
}

void func_801b1740_slot04_16(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    obj->field_160 = 0x64;
    obj->field_46 = 0;
    ((Slot04bObj *)obj)->field_1c0 = 0;
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x1a);
}

void func_801b17a8_slot04_16(Object *obj) {
    u16 a;
    int i;

    ref_other.p = obj->other;
    a = obj->field_3a;
    if ((a & 0xff) == 1) {
        obj->field_3a = a & 0xff00;
        func_80146478(obj, 1, -0x69, 0x49);
        i = 0;
        if (((Slot04bObj *)obj)->field_1c0 == 0) {
            i = 0xb;
            ((Slot04bObj *)obj)->field_1c0 = ((Slot04bObj *)obj)->field_1c0 + 1;
        }
        if (func_80140cd8(obj, (s16)i, 0) != 0) {
            func_801204f4(obj, obj->side, 0x16);
            obj->field_07 = 3;
            func_801307e0(obj, 0x1b);
        } else {
            func_801204f4(obj, obj->side, 0x14);
            obj->field_46 = obj->field_46 + 1;
            func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            func_80130efc(obj);
        } else {
            obj->field_07 = 2;
            func_801307e0(obj, 0x1b);
        }
        }
    } else {
        func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            func_80130efc(obj);
        } else {
            obj->field_07 = 2;
            func_801307e0(obj, 0x1b);
        }
    }
}

void func_801b1930_slot04_16(Object *obj) {
    u8 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t == 1) {
        if (((Slot04bObj *)obj)->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, t);
        } else {
            func_80140770(obj, 0, 5, 0xb, 0, 0, t);
        }
        obj->field_07 = 4;
    }
}

void func_801b19ac_slot04_16(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;

    func_80130efc(obj);
    if (o->field_3a == 1) {
        if (o->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 0);
        } else {
            func_80140770(obj, 0, 5, 0xb, 0, 0, 0);
        }
        obj->field_07 = 4;
    }
}

void func_801b1a28_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801b10d0_slot04_16(obj);
    }
}

void func_801b1a68_slot04_16(Object *obj) {
    if (func_8013d210(obj) && (u8)func_801b235c_slot04_16(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b1c50_slot04_16(obj)) return;
    if (func_8013cd5c(obj, 0, 0x22) && func_801b1d38_slot04_16(obj)) return;
    if (func_8013caf0(obj, 1, 0x15) && func_801b1dc4_slot04_16(obj)) return;
    if (func_8013ccb4(obj, 2, 0x21) && func_801b1e64_slot04_16(obj)) return;
    if (func_8013ccb4(obj, 3, 0x23) && func_801b1ed0_slot04_16(obj)) return;
    if (func_8013cac8(obj, 4, 4) && func_801b1fc4_slot04_16(obj)) return;
    if (func_8013cac8(obj, 5, 0xd) && func_801b210c_slot04_16(obj)) return;
    if (func_8013cac8(obj, 6, 0xe) && (u8)func_801b2238_slot04_16(obj)) return;
    if ((u8)func_8013d0c8(obj) && func_801b2034_slot04_16(obj)) return;
    if ((u8)func_8013d0fc(obj)) func_801b20a4_slot04_16(obj);
}

u8 func_801b1c50_slot04_16(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->field_c6 >= 0x30) {
            if (obj->field_45 == 0) {
                if (func_80141e34(obj)) {
                    if (func_80141788(obj)) {
                        r = 1;
                        obj->field_04 = 1;
                        obj->field_05 = 0;
                        obj->field_06 = 8;
                        obj->field_07 = 0;
                        obj->field_15a = 0;
                        obj->field_0b = obj->field_158;
                    }
                }
            } else if (obj->field_7e == 0) {
                if (func_801418bc(obj)) {
                    r = 1;
                    obj->field_04 = 1;
                    obj->field_05 = 0;
                    obj->field_06 = 8;
                    obj->field_07 = 0;
                    obj->field_15a = 0;
                }
            }
        }
        return r;
    }
    return 0;
}
