/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c64ec_slot04_14[];
extern ObjectFn data_801c64f8_slot04_14[];
extern ObjectFn data_801c650c_slot04_14[];
extern u8 data_801c651c_slot04_14[];
extern u16 data_801c652c_slot04_14[];
extern ObjectFn data_801c6538_slot04_14[];
extern u16 box_margin;

void func_801b5b80_slot04_14(Object *obj);
u8 func_801b5e44_slot04_14(Object *obj);
u8 func_801b5e7c_slot04_14(Object *obj);
void func_80130678(Object *object, int arg);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
u8 func_80125734(Object *object, int a);

void func_801b5a90_slot04_14(Object *obj) {
    data_801c64ec_slot04_14[obj->field_07](obj);
}

void func_801b5ad0_slot04_14(Object *obj) {
    Object *o = obj->other;
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(o, o->side, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (func_801b5e7c_slot04_14(obj) != 0) {
            obj->field_0b = obj->field_0b + 1;
        }
    } else if (obj->field_c2 & 0x8000) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, (obj->field_129 >> 1) + 0x18);
}

void func_801b5b80_slot04_14(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0xf, 0, 1, 0);
    }
    func_80130efc(obj);
}

void func_801b5be4_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5c34_slot04_14(Object *obj) {
    data_801c64f8_slot04_14[obj->field_07](obj);
}

void func_801b5c74_slot04_14(Object *obj) {
    Object *o = obj->other;
    int t;
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(o, o->side, 0x31a);
    obj->field_0b = 0;
    obj->field_4c = 0x48000;
    if (obj->field_cd != 0) {
        t = func_801b5e7c_slot04_14(obj);
    } else {
        t = obj->field_c2 & 0x8000;
    }
    if (t) {
        obj->field_4c = -0x48000;
        obj->field_0b = obj->field_0b + 1;
    }
    obj->field_50 = 0x20000;
    obj->field_58 = -0x2000;
    func_801307e0(obj, 0x45);
}

void func_801b5d34_slot04_14(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
    }
    func_80130efc(obj);
}

void func_801b5d74_slot04_14(Object *obj) {
    if (func_801b5e44_slot04_14(obj)) {
        if (obj->field_70 <= obj->pos_y) {
            obj->pos_y = obj->field_70;
            game_state.field_63 = 0x18;
        }
    }
    func_801b5b80_slot04_14(obj);
}

void func_801b5dd8_slot04_14(Object *obj) {
    if (func_801b5e44_slot04_14(obj)) {
        if (obj->field_70 <= obj->pos_y) {
            obj->pos_y = obj->field_70;
            game_state.field_63 = 0x18;
            obj->field_07 = obj->field_07 + 1;
        }
    }
}

u8 func_801b5e44_slot04_14(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    return obj->field_50 < 0;
}

u8 func_801b5e7c_slot04_14(Object *obj) {
    return (s16)(box_margin + 0xc0) < obj->pos_x;
}

void func_801b5e9c_slot04_14(Object *obj) {
    data_801c650c_slot04_14[obj->field_06](obj);
}

void func_801b5edc_slot04_14(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b5f10_slot04_14(Object *obj) {
    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b5f60_slot04_14(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_46 = 0x3c;
    game_state.field_76 = 0x1e;
    func_80130678(obj, data_801c652c_slot04_14[func_80125734(obj, data_801c651c_slot04_14[func_80151184() & 0xf])]);
}

void func_801b5fe8_slot04_14(Object *obj) {
    u16 t = obj->field_3a;
    s16 c;
    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
        game_state.field_63 = 0x18;
        func_801204f4(obj, obj->side, 0xe);
    }
    c = obj->field_46;
    if (c != 0) {
        c = c - 1;
        obj->field_46 = c;
        if (c == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    func_80130efc(obj);
}

void func_801b608c_slot04_14(Object *obj) {
    data_801c6538_slot04_14[obj->field_06](obj);
}

void func_801b60cc_slot04_14(Object *obj) {
    if (game_state.field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b6108_slot04_14(Object *obj) {
    int idx = 0x28;
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->other->field_0b;
    obj->field_46 = 0x78;
    if (game_state.field_a6 != 0) {
        idx = 0x29;
    }
    func_80130678(obj, idx);
}

void func_801b6160_slot04_14(Object *obj) {
    s16 t = obj->field_46;
    if (t != 0) {
        t = t - 1;
        obj->field_46 = t;
        if (t == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    func_80130efc(obj);
}

void func_801b61c4_slot04_14(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x57; i >= 0; i--) {
        *p++ = z;
    }
}
