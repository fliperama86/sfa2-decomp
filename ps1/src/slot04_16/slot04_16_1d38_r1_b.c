/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
u8 func_8013f8c4(Object *obj, int a, int b);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80146998(Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80130678(Object *object, int index);
void func_801b10b0_slot04_16(Object *obj);
void func_801b6434_slot04_16(Object *obj);

extern ObjectFn data_801ca168_slot04_16[];
extern ObjectFn data_801ca198_slot04_16[];
extern ObjectFn data_801ca1a8_slot04_16[];
extern ObjectFn data_801ca1b0_slot04_16[];
extern ObjectFn data_801ca1c0_slot04_16[];
extern ObjectFn data_801ca1c8_slot04_16[];

int func_801b235c_slot04_16(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->field_177 == 0) {
            return 0;
        }
    }
    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 1;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
    }
    return r;
}

void func_801b23e8_slot04_16(Object *obj) {
    data_801ca168_slot04_16[obj->field_15a](obj);
}

void func_801b2428_slot04_16(Object *obj) {
    data_801ca198_slot04_16[obj->field_07](obj);
}

void func_801b2468_slot04_16(Object *obj) {
    int a;
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38((GameState *)game_state.config, obj);
    if (obj->field_45 != 0) {
        a = 0x2f;
    } else {
        a = 0x2b;
    }
    func_80130678(obj, a);
}

void func_801b24d4_slot04_16(Object *obj) {
    s16 a;
    int b;
    if ((s16)obj->field_3a & 0xff00) {
        a = 0;
        b = 0x55;
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = -0x24;
            b = 0x53;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b6434_slot04_16(obj);
    }
    func_80130efc(obj);
}

void func_801b2574_slot04_16(Object *obj) {
    u8 a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_46 = (u8)obj->field_46 | 0x1400;
        if (obj->field_4b != 0) {
            obj->field_46 = (obj->field_46 >> 8 << 8) + 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b2610_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;
    int t;

    obj->field_27c++;
    func_80130efc(o);
    a = o->field_46;
    t = a - 1;
    o->field_46 = t;
    if ((t & 0xff) == 0) {
        if (o->field_45 != 0) {
            o->field_04 = 1;
            o->field_05 = 0;
            o->field_06 = 3;
            o->field_07 = 1;
        }
        func_80142c70(o);
    } else {
        if (o->field_134 != 0) {
            if (obj->field_27d == 0) {
                obj->field_27d = obj->field_27c;
            }
        } else if ((s16)(t & -0x100) != 0) {
            o->field_46 = a - 0x101;
            if ((o->field_46 & 0x100) == 0) {
                return;
            }
        } 
        func_80142fe8(o);
    }
}

void func_801b26e8_slot04_16(Object *obj) {
    data_801ca1a8_slot04_16[obj->field_07](obj);
}

void func_801b2728_slot04_16(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_159 = 0;
    obj->field_157 = 0;
    func_801307e0(obj, 0x45);
}

void func_801b277c_slot04_16(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b10b0_slot04_16(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b27bc_slot04_16(Object *obj) {
    data_801ca1b0_slot04_16[obj->field_07](obj);
}

void func_801b27fc_slot04_16(Object *obj) {
    if ((u8)obj->field_3a) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a &= 0xff00;
        func_80146998(obj);
    }
    func_80130efc(obj);
}

void func_801b2854_slot04_16(Object *obj) {
    if ((u8)obj->field_3a != 0) {
        if (func_8013f8c4(obj, -0x26, 0x3a) != 0) {
            obj->field_07++;
            func_80120554(obj, obj->side ^ 1, 0x31a);
        } else {
            obj->field_07 = 3;
            func_801307e0(obj, 0x21);
            return;
        }
    }
    func_80130efc(obj);
}

void func_801b28e0_slot04_16(Object *obj) {
    u16 t = obj->field_3a;
    u8 u;
    if ((t & 0xff) == 2) {
        obj->field_3a = t & 0xff00;
        func_80146478(obj, 1, -0x46, 0x45);
        func_80120554(obj, obj->side ^ 1, 0x304);
    }
    u = obj->field_3a;
    if (u == 1) {
        obj->field_07 = obj->field_07 + 1;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->field_260 = 1;
        func_80140770(obj, 0, 0x11, 0x11, 4, 0, u);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            obj->field_167 = 5;
        }
    }
    func_80130efc(obj);
}

void func_801b29e4_slot04_16(Object *obj) {
    s16 t;
    int a;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t >= 0) {
        if ((u8)t == 3) {
            a = 0x30;
            obj->field_3a = t & 0xff00;
            if (obj->field_0b == 0) {
                a = -0x30;
            }
            obj->pos_x = a + obj->pos_x;
        }
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b2a78_slot04_16(Object *obj) {
    data_801ca1c0_slot04_16[obj->field_07](obj);
}

void func_801b2ab8_slot04_16(Object *obj) {
    if ((u8)obj->field_3a) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a &= 0xff00;
        func_80146998(obj);
    }
    func_80130efc(obj);
}

void func_801b2b10_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b2b6c_slot04_16(Object *obj) {
    data_801ca1c8_slot04_16[obj->field_07](obj);
}
