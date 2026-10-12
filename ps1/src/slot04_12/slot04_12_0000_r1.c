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
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_80142a14(Object *object);
void set_sequence_by_side(Object *object, int index);
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
    obj->data_ptr_a = data_801b4de8_slot04_12;
    obj->data_ptr_b = data_801b7604_slot04_12;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x29; i++) {
        dst[i] = data_801c4948_slot04_12[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x12;
        c->table_index = 0;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        c->sound_side = obj->sound_side;
        c->state_3 = obj->kind;
        *(u32 *)&obj->linked_object_2 = (u32)c;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x12;
        c->table_index = 1;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        c->state_3 = obj->kind;
        c->sound_side = obj->sound_side;
        obj->linked_object = (u32)c;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
    }
}

void func_801b01bc_slot04_12(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    } else {
        update_sequence(obj);
    }
}

void func_801b0210_slot04_12(Object *obj) {
    data_801c49ec_slot04_12[obj->state_2](obj);
}

void func_801b0250_slot04_12(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void func_801b0284_slot04_12(Object *obj) {
    if (game_state.field_64 == 0 || game_state.countdown_timer == 0) {
        obj->state_2++;
    }
    update_sequence(obj);
}

void func_801b02d4_slot04_12(Object *obj) {
    s16 k;

    obj->countdown = 0x3c;
    obj->state_2++;
    game_state.countdown = 0x1e;
    k = 3;
    if ((s16)obj->health != 0x90) {
        k = data_801c49fc_slot04_12[get_random_byte() & 0xf];
    }
    k = (s8)func_80125734(obj, (s8)k);
    k += 0x23;
    set_sequence_by_side(obj, k);
}

void func_801b0370_slot04_12(Object *obj) {
    s16 t = obj->countdown;
    if (t != 0) {
        t = t - 1;
        obj->countdown = t;
        if (t == 0) {
            game_state.side_bits_2 |= 1 << obj->side;
        }
    }
    update_sequence(obj);
}

void func_801b03d4_slot04_12(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b0a40_slot04_12(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b05a8_slot04_12(obj);
    } else {
        func_801b042c_slot04_12(obj);
    }
}

void func_801b042c_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b0528_slot04_12(obj);
    } else {
        func_801b046c_slot04_12(obj);
    }
}

void func_801b046c_slot04_12(Object *obj) {
    if (obj->state_3 != 0) {
        func_80142a14(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
            if ((u8)func_8013f8c4(obj, -0x14, 0xe) != 0) {
                obj->state_0 = 1;
                obj->state_1 = 2;
                obj->state_2 = 0;
                obj->state_3 = 0;
            } else {
                obj->active_flag_3 = 1;
                func_80130dc0(obj);
            }
        } else {
            obj->active_flag_3 = 1;
            func_80130dc0(obj);
        }
    }
}

void func_801b0528_slot04_12(Object *obj) {
    if (obj->state_3 != 0) {
        func_80142a14(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index == 4) {
            play_side_sound_at(obj, obj->side, 0xb);
        }
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b05a8_slot04_12(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b0634_slot04_12(obj);
    } else {
        func_801b05e8_slot04_12(obj);
    }
}

void func_801b05e8_slot04_12(Object *obj) {
    if (obj->state_3 != 0) {
        func_80142a14(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b0634_slot04_12(Object *obj) {
    data_801c4a0c_slot04_12[obj->state_3](obj);
}

void func_801b0674_slot04_12(Object *obj) {
    u8 t = obj->state_3;
    obj->state_3 = t + 1;
    if (obj->direction_index == 4 && obj->field_25f == 0 && (obj->input_bits_2 & 0x8000) != 0) {
        obj->state_3++;
        obj->active_flag_3 = 1;
        func_80141f28(obj, 2);
        obj->vel_y = 0x78000;
        obj->accel_x = 0;
        obj->accel_y = -0x5000;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x18000;
        } else {
            obj->vel_x = -0x18000;
        }
        obj->pos_y = obj->pos_y - 0x10;
        obj->active_state = 1;
        obj->field_157 = 0;
        func_80130ec0(obj);
        obj->field_278 = 1;
        func_801307e0(obj, 0x1b);
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b0770_slot04_12(Object *obj) {
    func_80142a14(obj);
}

void func_801b0790_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (func_801b3a4c_slot04_12(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        if (obj->field_7e == 0) {
            obj->facing_side = obj->facing_side ^ 1;
        }
        func_801209c4(obj);
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b0820_slot04_12(Object *obj) {
    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    if (obj->input_dir_second != 0) {
        func_801b0904_slot04_12(obj);
    } else if (obj->direction_index != 0 && (obj->input_bits_2 & 0xe000) != 0 && obj->pos_y < obj->ground_y - 0x30 && (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b09ac_slot04_12(obj);
    }
}
