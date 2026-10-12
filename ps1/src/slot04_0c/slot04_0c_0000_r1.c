/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4824_slot04_0c[];
extern u8 data_801b5bf4_slot04_0c[];
extern u32 data_801bd828_slot04_0c[];
extern u16 data_801bd8c8_slot04_0c[];
extern ObjectFn data_801bd8cc_slot04_0c[];
extern s16 data_801bd8d8_slot04_0c[];
extern ObjectFn data_801bd8dc_slot04_0c[];
extern ObjectFn data_801bd8e4_slot04_0c[];
extern ObjectFn data_801bd8f0_slot04_0c[];

Object *func_8011f32c(void);
void set_sequence_by_side(Object *object, int arg);
void func_80130dc0(Object *object);
void func_80142a14(Object *object);

void func_801b01ac_slot04_0c(Object *obj);
void func_801b0300_slot04_0c(Object *obj);
void func_801b0340_slot04_0c(Object *obj);
void func_801b0430_slot04_0c(Object *obj);
void func_801b0590_slot04_0c(Object *obj);

void func_801b0000_slot04_0c(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    if (game_state.count_index == 0) {
        obj->pos_x = data_801bd8c8_slot04_0c[obj->side];
    }
    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b4824_slot04_0c;
    obj->data_ptr_b = data_801b5bf4_slot04_0c;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801bd828_slot04_0c[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0xc;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        obj->linked_object = (u32)c;
        c->sound_side = obj->side;
        c->image_row = obj->image_row;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}

void func_801b013c_slot04_0c(Object *obj) {
    data_801bd8cc_slot04_0c[obj->state_2](obj);
}

void func_801b017c_slot04_0c(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->vel_x = 0x80000;
    func_801b01ac_slot04_0c(obj);
}

void func_801b01ac_slot04_0c(Object *obj) {
    int a = 0x32;

    if (obj->facing_side == 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    }
    if (obj->pos_x != data_801bd8d8_slot04_0c[obj->side]) {
        update_sequence(obj);
    } else {
        obj->countdown = 0x48;
        obj->state_2 = obj->state_2 + 1;
        if (obj->other->kind == 0xb) {
            a = 0x33;
        }
        set_sequence_by_side(obj, a);
    }
}

void func_801b0260_slot04_0c(Object *obj) {
    s16 t;

    update_sequence(obj);
    t = obj->countdown;
    t -= 1;
    obj->countdown = t;
    if (t == 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    }
}

void func_801b02c0_slot04_0c(Object *obj) {
    if (obj->input_dir_0 == 0) {
        func_801b0300_slot04_0c(obj);
    } else {
        func_801b0590_slot04_0c(obj);
    }
}

void func_801b0300_slot04_0c(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_801b0340_slot04_0c(obj);
    } else {
        func_801b0430_slot04_0c(obj);
    }
}

void func_801b0340_slot04_0c(Object *obj) {
    data_801bd8dc_slot04_0c[obj->state_3](obj);
}

void func_801b0380_slot04_0c(Object *obj) {
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        obj->state_3 = obj->state_3 + 1;
        func_80130dc0(obj);
    }
}

void func_801b0410_slot04_0c(Object *obj) {
    func_80142a14(obj);
}

void func_801b0430_slot04_0c(Object *obj) {
    u16 t;

    data_801bd8e4_slot04_0c[obj->state_3](obj);
    t = obj->step_flags;
    if ((t & 0x7f00) != 0) {
        obj->step_flags = t & 0x80ff;
        play_sound_at(obj, obj->side, 0x31f);
    }
}

void func_801b04a4_slot04_0c(Object *obj) {
    int one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index == 4) {
        obj->state_3 = 2;
        obj->field_278 = one;
    }
    func_80130dc0(obj);
}

void func_801b04f0_slot04_0c(Object *obj) {
    func_80142a14(obj);
}

void func_801b0510_slot04_0c(Object *obj) {
    s16 t = obj->step_flags;
    int d = 0x80000;

    if ((t & 0x8000) != 0) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            obj->step_flags = t & 0xff00;
            if (obj->facing_side == 0) {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - d;
            } else {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + d;
            }
        }
        update_sequence(obj);
    }
}

void func_801b0590_slot04_0c(Object *obj) {
    obj->field_157 = 1;
    data_801bd8f0_slot04_0c[obj->state_3](obj);
}

void func_801b05d4_slot04_0c(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}

void func_801b0604_slot04_0c(Object *obj) {
    func_80142a14(obj);
}

void func_801b0624_slot04_0c(Object *obj) {
    int x = 0xc;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        x = 0x12;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    func_801307e0(obj, (s16)((obj->direction_index >> 1) + x));
}
