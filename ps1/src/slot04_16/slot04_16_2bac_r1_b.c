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
int func_80130184(Object *object);
void func_80145f98(Object *object);
void func_801b10b0_slot04_16(Object *obj);
void func_801b2fd0_slot04_16(Object *obj);
void func_801b3258_slot04_16(Object *obj);
void func_801b34f8_slot04_16(Object *obj);

void func_801b2fd0_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b3024_slot04_16(Object *obj) {
    data_801ca1e8_slot04_16[obj->state_3](obj);
}

void func_801b3064_slot04_16(Object *obj) {
    int a;

    obj->field_17b = 1;
    ((Slot04bObj *)obj)->field_298 = 0;
    obj->field_225 = 1;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8((GameState *)data_80190468.p, obj);
    a = 0x49;
    if (obj->field_49 != 0) {
        a = 0x53;
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
    func_801b3258_slot04_16(obj);
}

void func_801b30ec_slot04_16(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = *(u8 *)&obj->step_flags;
    if (t == 0) {
        return;
    }
    if (t != 3) {
        func_801b3258_slot04_16(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    func_801b3258_slot04_16(obj);
    game_state.other_object = pop_object_from_pool(obj);
    if (game_state.other_object != 0) {
        game_state.other_object->field_00 = 1;
        game_state.other_object->field_02 = 0x1e;
        game_state.other_object->field_66 = obj->field_66;
        game_state.other_object->field_65 = obj->field_65;
        game_state.other_object->field_ac = obj->field_12a;
        game_state.other_object->field_ad = 0;
        game_state.other_object->field_0e = obj->field_0e;
        game_state.other_object->field_26 = obj->field_26;
        game_state.other_object->pos_x = obj->pos_x;
        game_state.other_object->pos_y = obj->pos_y;
        game_state.other_object->field_0b = obj->field_0b;
        game_state.other_object->health = 0;
        game_state.other_object->field_3c = obj;
    }
}

void func_801b3258_slot04_16(Object *obj) {
    u16 tbl[20] = { 0xe, 0x10, 0x14, 0, 1, 3, 7, 0, 3, 5, 9, 0, 0x16, 0x18, 0x1c, 0, 8, 0x10, 0x18, 0 };
    u16 d;

    d = tbl[*(u8 *)&obj->step_flags * 4 + (obj->field_12a >> 1)];
    if (obj->field_0b == 0) {
        d = -d;
    }
    obj->pos_x = obj->pos_x + d;
    obj->step_flags = obj->step_flags & 0xff00;
}

void func_801b3374_slot04_16(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 4) {
        obj->field_17b = 0;
        obj->state_3 = obj->state_3 + 1;
        func_801b3258_slot04_16(obj);
    }
}

void func_801b33c0_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b3414_slot04_16(Object *obj) {
    data_801ca1f8_slot04_16[obj->state_3](obj);
}

void func_801b3454_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int one = 1;

    o->field_17b = one;
    obj->field_298 = 0;
    o->state_3 = o->state_3 + 1;
    game_state.other_object = o->other;
    game_state.other_object->field_249 = 0;
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
    if ((u8)func_8013f8c4(obj, -0x26, (s16)(x - d)) == 0) {
        obj->state_3 = 7;
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        func_80141f28(obj, 0x14);
        func_80138ae8((GameState *)game_state.config, obj);
        if (obj->field_49 != 0) {
            func_80145f98(obj);
        }
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        func_801307e0(obj, 0x1d);
    }
}

void func_801b362c_slot04_16(Object *obj) {
    s32 tbl[3][4] = { { 0x11000, 0x98000, 0x2000, -0x8000 }, { 0x14000, 0xac000, 0x1000, -0x6000 }, { 0x14000, 0xa8000, 0x1000, -0x4000 } };

    if (*(u8 *)&obj->step_flags != 1) {
        update_sequence(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    obj->field_45 = 1;
    obj->step_flags = obj->step_flags & 0xff00;
    obj->vel_x = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 0);
    obj->vel_y = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 4);
    obj->accel_x = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 8);
    obj->accel_y = *(s32 *)((((u32)obj->field_12a >> 1) << 4) + (u32)tbl + 12);
    if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
        if (obj->field_0b != 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        } else {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    } else if (obj->field_0b != 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}

void func_801b379c_slot04_16(Object *obj) {
    s32 tbl[3] = { -0x9000, -0xc000, -0xc000 };

    func_80130184(obj);
    if (obj->vel_y >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->accel_y = tbl[obj->field_12a >> 1];
        obj->accel_x = 0;
        update_sequence(obj);
    }
}

void func_801b383c_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->field_45 = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801307e0(obj, 0x1e);
    }
}
