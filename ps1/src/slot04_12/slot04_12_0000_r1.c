/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4de8_slot04_12[];
extern u8 data_801b7604_slot04_12[];
extern u32 data_801c4948_slot04_12[];
extern ObjectFn data_801c49ec_slot04_12[];
extern u8 data_801c49fc_slot04_12[];
extern ObjectFn data_801c4a0c_slot04_12[];

Object *func_8011f32c(void);
u8 func_8013f8c4(Object *object, int a, int b);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_80142a14(Object *object);
u8 func_80125734(Object *object, int a);
void func_80130678(Object *object, int index);
void func_80130dc0(Object *object);

void func_801b042c_slot04_12(Object *obj);
void func_801b05a8_slot04_12(Object *obj);
void func_801b0528_slot04_12(Object *obj);
void func_801b046c_slot04_12(Object *obj);
void func_801b0634_slot04_12(Object *obj);
void func_801b05e8_slot04_12(Object *obj);
void func_801b0a40_slot04_12(Object *obj);
void func_801b0904_slot04_12(Object *obj);
void func_801b09ac_slot04_12(Object *obj);
int func_801b3a4c_slot04_12(Object *obj);

void func_801b0000_slot04_12(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b4de8_slot04_12;
    obj->field_9c = data_801b7604_slot04_12;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x29; i++) {
        dst[i] = data_801c4948_slot04_12[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x12;
        c->field_03 = 0;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_0e = obj->field_0e;
        c->field_66 = obj->field_66;
        c->field_07 = obj->kind;
        *(u32 *)&obj->field_2c = (u32)c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x12;
        c->field_03 = 1;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_0e = obj->field_0e;
        c->field_07 = obj->kind;
        c->field_66 = obj->field_66;
        obj->field_28 = (u32)c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
    }
}

void func_801b01bc_slot04_12(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        func_80130678(obj, 0);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0210_slot04_12(Object *obj) {
    data_801c49ec_slot04_12[obj->field_06](obj);
}

void func_801b0250_slot04_12(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0284_slot04_12(Object *obj) {
    if (game_state.field_64 == 0 || game_state.field_5c == 0) {
        obj->field_06++;
    }
    func_80130efc(obj);
}

void func_801b02d4_slot04_12(Object *obj) {
    s16 k;

    obj->field_46 = 0x3c;
    obj->field_06++;
    game_state.field_76 = 0x1e;
    k = 3;
    if ((s16)obj->field_5c != 0x90) {
        k = data_801c49fc_slot04_12[func_80151184() & 0xf];
    }
    k = (s8)func_80125734(obj, (s8)k);
    k += 0x23;
    func_80130678(obj, k);
}

void func_801b0370_slot04_12(Object *obj) {
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

void func_801b03d4_slot04_12(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b0a40_slot04_12(obj);
    } else if (obj->field_128 != 0) {
        func_801b05a8_slot04_12(obj);
    } else {
        func_801b042c_slot04_12(obj);
    }
}

void func_801b042c_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b0528_slot04_12(obj);
    } else {
        func_801b046c_slot04_12(obj);
    }
}

void func_801b046c_slot04_12(Object *obj) {
    if (obj->field_07 != 0) {
        func_80142a14(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
            if (func_8013f8c4(obj, -0x14, 0xe) != 0) {
                obj->field_04 = 1;
                obj->field_05 = 2;
                obj->field_06 = 0;
                obj->field_07 = 0;
            } else {
                obj->field_159 = 1;
                func_80130dc0(obj);
            }
        } else {
            obj->field_159 = 1;
            func_80130dc0(obj);
        }
    }
}

void func_801b0528_slot04_12(Object *obj) {
    if (obj->field_07 != 0) {
        func_80142a14(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_12a == 4) {
            func_801204f4(obj, obj->side, 0xb);
        }
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b05a8_slot04_12(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b0634_slot04_12(obj);
    } else {
        func_801b05e8_slot04_12(obj);
    }
}

void func_801b05e8_slot04_12(Object *obj) {
    if (obj->field_07 != 0) {
        func_80142a14(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b0634_slot04_12(Object *obj) {
    data_801c4a0c_slot04_12[obj->field_07](obj);
}

void func_801b0674_slot04_12(Object *obj) {
    u8 t = obj->field_07;
    obj->field_07 = t + 1;
    if (obj->field_12a == 4 && obj->field_25f == 0 && (obj->field_130 & 0x8000) != 0) {
        obj->field_07++;
        obj->field_159 = 1;
        func_80141f28(obj, 2);
        obj->field_50 = 0x78000;
        obj->field_54 = 0;
        obj->field_58 = -0x5000;
        if (obj->field_0b != 0) {
            obj->field_4c = 0x18000;
        } else {
            obj->field_4c = -0x18000;
        }
        obj->pos_y = obj->pos_y - 0x10;
        obj->field_45 = 1;
        obj->field_157 = 0;
        func_80130ec0(obj);
        obj->field_278 = 1;
        func_801307e0(obj, 0x1b);
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b0770_slot04_12(Object *obj) {
    func_80142a14(obj);
}

void func_801b0790_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (func_801b3a4c_slot04_12(obj) < 0 && obj->pos_y >= obj->field_70) {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        if (obj->field_7e == 0) {
            obj->field_0b = obj->field_0b ^ 1;
        }
        func_801209c4(obj);
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0820_slot04_12(Object *obj) {
    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80130504(obj);
    if (obj->field_129 != 0) {
        func_801b0904_slot04_12(obj);
    } else if (obj->field_12a != 0 && (obj->field_130 & 0xe000) != 0 && obj->pos_y < obj->field_70 - 0x30 && (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b09ac_slot04_12(obj);
    }
}
