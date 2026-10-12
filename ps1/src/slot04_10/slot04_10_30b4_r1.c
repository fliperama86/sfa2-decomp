/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern ObjectFn data_801c6a64_slot04_10[];
void func_80142adc(Object *object);
int func_80130184(Object *object);
void func_80145f98(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b1078_slot04_10(Object *object);
void func_801b3220_slot04_10(Object *object);
void func_801b34c0_slot04_10(Object *object);

void func_801b30b4_slot04_10(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = *(u8 *)&obj->step_flags;
    if (t == 0) {
        return;
    }
    if (t != 3) {
        func_801b3220_slot04_10(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    func_801b3220_slot04_10(obj);
    game_state.other_object = pop_object_from_pool(obj);
    if (game_state.other_object != 0) {
        game_state.other_object->active_count = 1;
        game_state.other_object->field_02 = 0x10;
        game_state.other_object->sound_side = obj->sound_side;
        game_state.other_object->team_id = obj->team_id;
        game_state.other_object->vel_table_index = obj->direction_index;
        game_state.other_object->active_flag_2 = 0;
        game_state.other_object->field_0e = obj->field_0e;
        game_state.other_object->scroll_factor = obj->scroll_factor;
        game_state.other_object->pos_x = obj->pos_x;
        game_state.other_object->pos_y = obj->pos_y;
        game_state.other_object->facing_side = obj->facing_side;
        game_state.other_object->health = 0;
        game_state.other_object->parent = obj;
    }
}

void func_801b3220_slot04_10(Object *obj) {
    u16 tbl[20] = { 0xe, 0x10, 0x14, 0, 1, 3, 7, 0, 3, 5, 9, 0, 0x16, 0x18, 0x1c, 0, 8, 0x10, 0x18, 0 };
    u16 d;

    d = tbl[*(u8 *)&obj->step_flags * 4 + (obj->direction_index >> 1)];
    if (obj->facing_side == 0) {
        d = -d;
    }
    obj->pos_x = obj->pos_x + d;
    obj->step_flags = obj->step_flags & 0xff00;
}

void func_801b333c_slot04_10(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 4) {
        obj->active_flag_4 = 0;
        obj->state_3 = obj->state_3 + 1;
        func_801b3220_slot04_10(obj);
    }
}

void func_801b3388_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        func_801b1078_slot04_10(obj);
    }
}

void func_801b33dc_slot04_10(Object *obj) {
    data_801c6a64_slot04_10[obj->state_3](obj);
}

void func_801b341c_slot04_10(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int one = 1;

    o->active_flag_4 = one;
    obj->field_298 = 0;
    o->state_3 = o->state_3 + 1;
    game_state.other_object = o->other;
    game_state.other_object->other_state_code = 0;
    func_80141f28(o, 5);
    func_80138ae8((GameState *)game_state.config, o);
    func_801307e0(o, (o->direction_index >> 1) + 0x40);
    if (o->field_49 != 0) {
        o->field_225 = one;
    }
    func_801b34c0_slot04_10(o);
}

void func_801b34c0_slot04_10(Object *obj) {
    u16 tbl[4] = { 0x68, 0x65, 0x61, 0 };
    u16 tbl2[4] = { 0x68, 0x65, 0x61, 0 };
    u16 x;
    int d;

    x = tbl[(obj->direction_index & 0xfe) >> 1];
    if (obj->field_49 != 0) {
        x = tbl[((obj->direction_index & 0xfe) >> 1) + 4];
    }
    d = obj->other_state_code != 0;
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

void func_801b35f4_slot04_10(Object *obj) {
    s32 tbl[3][4] = { { 0x11000, 0x98000, 0x2000, -0x8000 }, { 0x14000, 0xac000, 0x1000, -0x6000 }, { 0x14000, 0xa8000, 0x1000, -0x4000 } };

    if (*(u8 *)&obj->step_flags != 1) {
        update_sequence(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    obj->active_state = 1;
    obj->step_flags = obj->step_flags & 0xff00;
    obj->vel_x = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 0);
    obj->vel_y = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 4);
    obj->accel_x = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 8);
    obj->accel_y = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 12);
    if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
        if (obj->facing_side != 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        } else {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    } else if (obj->facing_side != 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}

void func_801b3764_slot04_10(Object *obj) {
    s32 tbl[3] = { -0x9000, -0xc000, -0xc000 };

    func_80130184(obj);
    if (obj->vel_y >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->accel_y = tbl[obj->direction_index >> 1];
        obj->accel_x = 0;
        update_sequence(obj);
    }
}

void func_801b3804_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801307e0(obj, 0x1e);
    }
}
