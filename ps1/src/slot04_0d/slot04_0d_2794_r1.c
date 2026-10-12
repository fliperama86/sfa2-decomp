/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2824_slot04_0d[];
extern ObjectFn data_801c2a90_slot04_0d[];
extern ObjectFn data_801c2aa4_slot04_0d[];
extern ObjectFn data_801c2aac_slot04_0d[];

void func_801483a4(Object *object, int a, int b);
void set_position_from_other_box(Object *object);
void func_801428e4(Object *object);
void func_80145d20(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801b2d40_slot04_0d(Object *object);
void func_801b2e9c_slot04_0d(Object *object);
void func_801b33c0_slot04_0d(Object *object);

void func_801b28f8_slot04_0d(Object *obj) {
    int v;

    obj->other_state_code = 2;
    if (obj->vel_x >= 0) {
        if ((u8)obj->step_flags == 0) {
            obj->active_state = 1;
            *(s32 *)&obj->pos_y_frac += obj->vel_y;
            obj->vel_y += obj->accel_y;
        }
        v = obj->vel_x;
        if (obj->facing_side == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b29a4_slot04_0d(Object *obj) {
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y > 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b29fc_slot04_0d(Object *obj) {
    s16 y;

    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    y = obj->ground_y;
    if (obj->pos_y >= y) {
        obj->active_state = 0;
        obj->pos_y = y;
        obj->pos_y_frac = 0;
        obj->active_flag_3 = 0;
        obj->state_3++;
        func_801209c4(obj);
    } else if ((u8)obj->step_flags != 0) {
        return;
    }
    update_sequence(obj);
}

void func_801b2a9c_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        func_801312b8(obj);
    }
}

void func_801b2ae0_slot04_0d(Object *obj) {
    data_801c2a90_slot04_0d[obj->state_3](obj);
}

void func_801b2b20_slot04_0d(Object *obj) {
    obj->field_157 = 1;
    obj->state_3++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x3b);
}

void func_801b2b88_slot04_0d(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        int a = -1;

        obj->state_3++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x1a, 0x44);
    }
}

void func_801b2bfc_slot04_0d(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        u8 i = 0;

        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        ((Slot04bObj *)obj)->table_value = data_801c2824_slot04_0d[i];
        obj->vel_x = 0x30000;
        obj->accel_x = 0;
    }
}

void func_801b2c8c_slot04_0d(Object *obj) {
    u16 a;

    update_sequence(obj);
    a = obj->step_flags;
    if ((a & 0xff) != 0) {
        obj->step_flags = a & 0xff00;
        obj->state_3++;
        obj->accel_x = -0x4000;
    }
    func_801b2d40_slot04_0d(obj);
}

void func_801b2ce8_slot04_0d(Object *obj) {
    update_sequence(obj);
    if ((s16)obj->step_flags >= 0) {
        func_801b2d40_slot04_0d(obj);
    } else {
        obj->other->other_state_code = 5;
        func_801312b8(obj);
    }
}

void func_801b2d40_slot04_0d(Object *obj) {
    int v = obj->vel_x;

    if (v >= 0) {
        if (obj->facing_side == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
}

void func_801b2d8c_slot04_0d(Object *obj) {
    data_801c2aa4_slot04_0d[obj->state_3](obj);
}

void func_801b2dcc_slot04_0d(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->field_157 = 0;
    func_801307e0(obj, 0x1c);
}

void func_801b2e1c_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2e5c_slot04_0d(Object *obj) {
    if (obj->input_dir_second != 0) {
        func_801b33c0_slot04_0d(obj);
    } else {
        func_801b2e9c_slot04_0d(obj);
    }
}

void func_801b2e9c_slot04_0d(Object *obj) {
    data_801c2aac_slot04_0d[obj->state_3](obj);
}

void func_801b2edc_slot04_0d(Object *obj) {
    obj->gauge = 0xb4;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->field_1cd = 0;
    obj->state_3++;
    func_80141f28(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x18);
    set_position_from_other_box(obj);
}
