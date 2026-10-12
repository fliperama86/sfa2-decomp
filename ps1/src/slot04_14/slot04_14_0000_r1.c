/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b8580_slot04_14[];
extern u8 data_801ba928_slot04_14[];
extern u32 data_801c60c8_slot04_14[];
extern ObjectFn data_801c6168_slot04_14[];
extern ObjectFn data_801c6170_slot04_14[];
extern ObjectFn data_801c617c_slot04_14[];
extern ObjectFn data_801c6184_slot04_14[];
extern ObjectFn data_801c6190_slot04_14[];

Object *func_8011f32c(void);
Block172 *func_8011f1e0(void);
void set_sequence_by_side(Object *object, int index);
void func_80130dc0(Object *object);
void func_80142a14(Object *object);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b0000_slot04_14(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b8580_slot04_14;
    obj->data_ptr_b = data_801ba928_slot04_14;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c60c8_slot04_14[i];
    }
    game_state.check_flag = 0;
    game_state.field_13c = 0;
    obj->field_252 = 0;
    c = func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x1a;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        s = obj->side;
        obj->linked_object = (u32)c;
        c->sound_side = s;
        c->image_row = obj->image_row;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}

void func_801b0128_slot04_14(Object *obj) {
    data_801c6168_slot04_14[obj->state_2](obj);
}

void func_801b0168_slot04_14(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    set_sequence_by_side(obj, 0x30);
}

void func_801b0194_slot04_14(Object *obj) {
    s16 t = obj->step_flags;
    Object *c;
    u8 e;

    if (t < 0) {
        func_801b62a8_slot04_14(obj, 1, 0, 0, 0);
        set_sequence_by_side(obj, 0);
    } else {
        if ((t & 0xff00) == 0x100) {
            obj->step_flags = t & 0x80ff;
            game_state.state_param = 0x18;
            play_side_sound_at(obj, obj->side, 0xe);
            c = (Object *)func_8011f1e0();
            if (c != 0) {
                c->active_count = 1;
                c->field_02 = 0x7d;
                c->parent = obj;
                c->field_0c = obj->field_0c;
                c->image_row = obj->image_row;
                e = obj->field_0e;
                obj->parent = c;
                c->field_02 = 0x12;
                c->mode_id = 0x20;
                c->field_0e = e;
                c->sound_side = obj->sound_side;
            }
        }
        update_sequence(obj);
    }
}

void func_801b028c_slot04_14(Object *obj) {
    data_801c6170_slot04_14[obj->input_dir_0 >> 1](obj);
}

void func_801b02d0_slot04_14(Object *obj) {
    data_801c617c_slot04_14[obj->input_dir_second >> 1](obj);
}

void func_801b0314_slot04_14(Object *obj) {
    data_801c6184_slot04_14[obj->state_3](obj);
}

void func_801b0354_slot04_14(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b62a8_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->active_flag_3 = one;
            obj->state_3 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->sequence_hold_flag = one;
            obj->field_278 = one;
            func_801307e0(obj, 0x27);
            return;
        }
    }
    obj->active_flag_3 = one;
    func_80130dc0(obj);
}

void func_801b0470_slot04_14(Object *obj) {
    func_80142a14(obj);
}

void func_801b0490_slot04_14(Object *obj) {
    s16 t = obj->step_flags;
    int d;

    if (t < 0) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            d = -0x20000;
            if (obj->facing_side != 0) {
                d = 0x20000;
            }
            *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        }
        update_sequence(obj);
    }
}

void func_801b0500_slot04_14(Object *obj) {
    data_801c6190_slot04_14[obj->state_3](obj);
}

void func_801b0540_slot04_14(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b62a8_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0) {
            obj->state_3 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            obj->vel_x = -0x36000;
            if (obj->facing_side != 0) {
                obj->vel_x = 0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            func_80130ec0(obj);
            obj->sequence_hold_flag = one;
            obj->field_278 = one;
            obj->active_flag_3 = one;
            func_801307e0(obj, 0x40);
            return;
        }
    }
    obj->active_flag_3 = one;
    func_80130dc0(obj);
}
