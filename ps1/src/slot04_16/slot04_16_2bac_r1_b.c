/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ca1e8_slot04_16[];
extern ObjectFn data_801ca1f8_slot04_16[];
extern ObjectRef data_80190468;
extern u16 box_margin[];

void func_80138ae8(GameState *state, Object *object);
void func_80142adc(Object *object);
Object *func_8011f0e8(void);
u8 func_8013f8c4(Object *object, int a, int b);
int func_80130184(Object *object);
void func_80145f98(Object *object);
void func_801b10b0_slot04_16(Object *obj);
void func_801b2fd0_slot04_16(Object *obj);
void func_801b3258_slot04_16(Object *obj);
void func_801b34f8_slot04_16(Object *obj);

void func_801b2fd0_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b3024_slot04_16(Object *obj) {
    data_801ca1e8_slot04_16[obj->field_07](obj);
}

void func_801b3064_slot04_16(Object *obj) {
    int a;

    obj->field_17b = 1;
    ((Slot04bObj *)obj)->field_298 = 0;
    obj->field_225 = 1;
    obj->field_07++;
    func_80141f28(obj, 7);
    func_80138ae8((GameState *)data_80190468.p, obj);
    a = 0x49;
    if (obj->field_49 != 0) {
        a = 0x53;
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
    func_801b3258_slot04_16(obj);
}

/* The call of func_8011f0e8 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 1 instruction slots. */
void func_801b30ec_slot04_16(Object *obj) {
    u8 t;

    func_80130efc(obj);
    t = *(u8 *)&obj->field_3a;
    if (t == 0) {
        return;
    }
    if (t != 3) {
        func_801b3258_slot04_16(obj);
        return;
    }
    obj->field_07 = obj->field_07 + 1;
    func_801b3258_slot04_16(obj);
    game_state.field_358 = ((Object *(*)(Object *))func_8011f0e8)(obj);
    if (game_state.field_358 != 0) {
        game_state.field_358->field_00 = 1;
        game_state.field_358->field_02 = 0x1e;
        game_state.field_358->field_66 = obj->field_66;
        game_state.field_358->field_65 = obj->field_65;
        game_state.field_358->field_ac = obj->field_12a;
        game_state.field_358->field_ad = 0;
        game_state.field_358->field_0e = obj->field_0e;
        game_state.field_358->field_26 = obj->field_26;
        game_state.field_358->pos_x = obj->pos_x;
        game_state.field_358->pos_y = obj->pos_y;
        game_state.field_358->field_0b = obj->field_0b;
        game_state.field_358->field_5c = 0;
        game_state.field_358->field_3c = obj;
    }
}

void func_801b3258_slot04_16(Object *obj) {
    u16 tbl[20] = { 0xe, 0x10, 0x14, 0, 1, 3, 7, 0, 3, 5, 9, 0, 0x16, 0x18, 0x1c, 0, 8, 0x10, 0x18, 0 };
    u16 d;

    d = tbl[*(u8 *)&obj->field_3a * 4 + (obj->field_12a >> 1)];
    if (obj->field_0b == 0) {
        d = -d;
    }
    obj->pos_x = obj->pos_x + d;
    obj->field_3a = obj->field_3a & 0xff00;
}

void func_801b3374_slot04_16(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 4) {
        obj->field_17b = 0;
        obj->field_07 = obj->field_07 + 1;
        func_801b3258_slot04_16(obj);
    }
}

void func_801b33c0_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b3414_slot04_16(Object *obj) {
    data_801ca1f8_slot04_16[obj->field_07](obj);
}

void func_801b3454_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int one = 1;

    o->field_17b = one;
    obj->field_298 = 0;
    o->field_07 = o->field_07 + 1;
    game_state.field_358 = o->other;
    game_state.field_358->field_249 = 0;
    func_80141f28(o, 5);
    func_80138ae8((GameState *)game_state.config, o);
    func_801307e0(o, (o->field_12a >> 1) + 0x40);
    if (o->field_49 != 0) {
        o->field_225 = one;
    }
    func_801b34f8_slot04_16(o);
}

void func_801b34f8_slot04_16(Object *obj) {
    u16 tbl[4] = { 0x68, 0x65, 0x61, 0 };
    u16 tbl2[4] = { 0x68, 0x65, 0x61, 0 };
    u16 x;
    int d;

    x = tbl[(obj->field_12a & 0xfe) >> 1];
    if (obj->field_49 != 0) {
        x = tbl[((obj->field_12a & 0xfe) >> 1) + 4];
    }
    d = obj->field_249 != 0;
    d <<= 4;
    d += 0x26;
    if (func_8013f8c4(obj, -0x26, (s16)(x - d)) == 0) {
        obj->field_07 = 7;
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        func_80141f28(obj, 0x14);
        func_80138ae8((GameState *)game_state.config, obj);
        if (obj->field_49 != 0) {
            func_80145f98(obj);
        }
        func_80120554(obj, obj->side ^ 1, 0x31a);
        func_801307e0(obj, 0x1d);
    }
}

void func_801b362c_slot04_16(Object *obj) {
    s32 tbl[3][4] = { { 0x11000, 0x98000, 0x2000, -0x8000 }, { 0x14000, 0xac000, 0x1000, -0x6000 }, { 0x14000, 0xa8000, 0x1000, -0x4000 } };

    if (*(u8 *)&obj->field_3a != 1) {
        func_80130efc(obj);
        return;
    }
    obj->field_07 = obj->field_07 + 1;
    obj->field_45 = 1;
    obj->field_3a = obj->field_3a & 0xff00;
    obj->field_4c = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 0);
    obj->field_50 = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 4);
    obj->field_54 = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 8);
    obj->field_58 = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 12);
    if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
        if (obj->field_0b != 0) {
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        } else {
            obj->field_4c = 0;
            obj->field_54 = 0;
        }
    } else if (obj->field_0b != 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
}

void func_801b379c_slot04_16(Object *obj) {
    s32 tbl[3] = { -0x9000, -0xc000, -0xc000 };

    func_80130184(obj);
    if (obj->field_50 >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_58 = tbl[obj->field_12a >> 1];
        obj->field_54 = 0;
        func_80130efc(obj);
    }
}

void func_801b383c_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801307e0(obj, 0x1e);
    }
}
