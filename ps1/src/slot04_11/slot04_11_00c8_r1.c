/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b744c_slot04_11[];
extern u8 data_801b9e04_slot04_11[];
extern u32 data_801c7244_slot04_11[];
extern ObjectFn data_801c72e4_slot04_11[];
extern s8 data_801c72f4_slot04_11[];
extern ObjectFn data_801c7304_slot04_11[];
extern ObjectFn data_801c7314_slot04_11[];
extern ObjectFn data_801c731c_slot04_11[];
extern s16 data_801c7324_slot04_11[];

Object *func_8011f32c(void);
void func_80130678(Object *object, int index);
u8 func_80125734(Object *object, int a);
u8 func_80151184(void);
u8 func_8013f8c4(Object *object, int a, int b);
int func_8013caf0(Object *object, u8 a, u8 b);
int func_8013cb70(Object *object, u8 a, u8 b);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013cdc8(Object *object, u8 a, u8 b);
int func_8013cfdc(Object *object, u8 a, u8 b);
int func_8013d1a8(Object *object);
u8 func_8013d210(Object *object);
void func_80142a14(Object *object);

/* functions of other units of this module */
void func_801b5ab8_slot04_11(Object *obj);
void func_801b5740_slot04_11(Object *obj);
int func_801b580c_slot04_11(Object *obj);
int func_801b1188_slot04_11(Object *obj);
int func_801b0a84_slot04_11(Object *obj);
int func_801b0b04_slot04_11(Object *obj);
int func_801b0b88_slot04_11(Object *obj);
int func_801b0c0c_slot04_11(Object *obj);
int func_801b0ca4_slot04_11(Object *obj);
int func_801b0d3c_slot04_11(Object *obj);
int func_801b0dd4_slot04_11(Object *obj);
int func_801b0e94_slot04_11(Object *obj);
int func_801b0f28_slot04_11(Object *obj);
int func_801b0fb0_slot04_11(Object *obj);
int func_801b109c_slot04_11(Object *obj);

int func_801b0a04_slot04_11(Object *obj);
void func_801b0688_slot04_11(Object *obj);
void func_801b0580_slot04_11(Object *obj);

void func_801b00d0_slot04_11(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b744c_slot04_11;
    obj->field_9c = data_801b9e04_slot04_11;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c7244_slot04_11[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x11;
        c->field_03 = 0;
        c->field_3c = obj;
        c->field_66 = obj->field_66;
        ((Slot04bObj *)obj)->field_2c = (u32)c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_0d = obj->field_0d;
    }
    obj->field_3c = 0;
    if (obj->kind == 0x13) {
        func_801b5ab8_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
    }
}

void func_801b01e4_slot04_11(Object *obj) {
    data_801c72e4_slot04_11[obj->field_06](obj);
}

void func_801b0224_slot04_11(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0258_slot04_11(Object *obj) {
    game_state.config = &game_state_second;
    if (game_state.config->field_64 == 0 && game_state.config->field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b02c0_slot04_11(Object *obj) {
    int v;

    ((Slot04bObj *)obj)->field_47 = 0x3c;
    obj->field_06 = obj->field_06 + 1;
    if (game_state.config->field_76 == 0) {
        game_state.config->field_76 = 0x1e;
    }
    v = (s8)func_80125734(obj, data_801c72f4_slot04_11[func_80151184() & 0xf]);
    v += 0x23;
    func_80130678(obj, v);
}

void func_801b0350_slot04_11(Object *obj) {
    if (((Slot04bObj *)obj)->field_47 != 0) {
        ((Slot04bObj *)obj)->field_47--;
        if (((Slot04bObj *)obj)->field_47 == 0) {
            game_state.config->field_4b |= obj->side + 1;
        }
    }
    func_80130efc(obj);
}

void func_801b03c0_slot04_11(Object *obj) {
    data_801c7304_slot04_11[obj->field_06](obj);
}

void func_801b0400_slot04_11(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0434_slot04_11(Object *obj) {
    if (game_state.config->field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b047c_slot04_11(Object *obj) {
    int arg = 0x28;

    ((Slot04bObj *)obj)->field_47 = 0x78;
    obj->field_06 = obj->field_06 + 1;
    if (game_state.config->field_a6 != 0) {
        arg = 0x29;
    }
    func_80130678(obj, arg);
}

void func_801b04d0_slot04_11(Object *obj) {
    if (((Slot04bObj *)obj)->field_47 != 0) {
        ((Slot04bObj *)obj)->field_47--;
        if (((Slot04bObj *)obj)->field_47 == 0) {
            game_state.config->field_4b |= obj->side + 1;
        }
    }
    func_80130efc(obj);
}

void func_801b0540_slot04_11(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b0688_slot04_11(obj);
    } else {
        func_801b0580_slot04_11(obj);
    }
}

void func_801b0580_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7314_slot04_11[obj->field_07](obj);
}

void func_801b05c0_slot04_11(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a == 0) {
        func_801b5740_slot04_11(obj);
    } else if (obj->field_25f != 0 || (obj->field_130 & 0xa000) == 0 ||
               func_8013f8c4(obj, -0x14, 0xe) == 0) {
        func_801b5740_slot04_11(obj);
    } else {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
}

void func_801b0668_slot04_11(Object *obj) {
    func_80142a14(obj);
}

void func_801b0688_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c731c_slot04_11[obj->field_07](obj);
}

void func_801b06cc_slot04_11(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_801b5740_slot04_11(obj);
}

void func_801b06f8_slot04_11(Object *obj) {
    func_80142a14(obj);
}

void func_801b0718_slot04_11(Object *obj) {
    int v;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80130504(obj);
    v = obj->field_12a;
    func_80141f28(obj, *(s16 *)((u8 *)data_801c7324_slot04_11 + (v & 0xfe)));
    v = 0xc;
    if (obj->field_48 != 0) {
        v = 0x12;
    }
    if (obj->field_129 != 0) {
        v += 3;
    }
    func_801307e0(obj, v + (obj->field_12a >> 1));
}

void func_801b07b8_slot04_11(Object *obj) {
    if (func_8013d1a8(obj) && func_801b1188_slot04_11(obj)) return;
    if (func_8013caf0(obj, 0, 0x17) && func_801b0dd4_slot04_11(obj)) return;
    if (func_8013cb70(obj, 1, 0x15) && func_801b0d3c_slot04_11(obj)) return;
    if (func_8013cb70(obj, 2, 0x14) && func_801b0ca4_slot04_11(obj)) return;
    if (func_8013caf0(obj, 3, 0x16) && func_801b0c0c_slot04_11(obj)) return;
    if (func_8013cdc8(obj, 4, 1) && func_801b0b88_slot04_11(obj)) return;
    if (func_8013cdc8(obj, 5, 2) && func_801b0b04_slot04_11(obj)) return;
    if (func_8013cac8(obj, 6, 5) && func_801b0a84_slot04_11(obj)) return;
    if (func_8013cfdc(obj, 7, 0) && func_801b0a04_slot04_11(obj)) return;
    if (func_801b580c_slot04_11(obj) && func_801b0e94_slot04_11(obj)) return;
    if (obj->kind == 0x11 && (obj->field_134 & 2) && func_801b0e94_slot04_11(obj)) return;
    if (obj->kind == 0x13 && (obj->field_134 & 1) && func_801b0e94_slot04_11(obj)) return;
    if (func_8013d210(obj) && func_801b0f28_slot04_11(obj)) return;
    if (func_8013cac8(obj, 8, 0xd) && func_801b0fb0_slot04_11(obj)) return;
    if (func_8013cac8(obj, 9, 0xe)) func_801b109c_slot04_11(obj);
}
