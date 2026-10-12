/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6378_slot04_14[];
extern s16 data_801c6390_slot04_14[];
extern ObjectFn data_801c6398_slot04_14[];
extern ObjectFn data_801c63a8_slot04_14[];

void set_sequence_by_side(Object *object, int arg);
void func_8013788c(Object *object);
void func_80145d20(Object *object);
void func_80157380(Object *object);
void func_801428e4(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801482e0(Object *parent, int x, int y);
int func_80130184(Object *object);
void func_801b3564_slot04_14(Object *obj);
void func_801b3654_slot04_14(Object *obj);

void func_801b340c_slot04_14(Object *obj) {
    int n;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    obj->field_29c = 2;
    obj->field_252 = 0xff;
    if (obj->mode_flag_2 != 0) {
        obj->variant_index = obj->facing_side ^ obj->direction_flag;
    }
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    obj->accel_x = -0x3000;
    obj->vel_x = 0xb8000;
    if (obj->input_dir_second != 0) {
        obj->vel_x = 0x80000;
    }
    n = 0x2d;
    if (obj->field_49 != 0) {
        n = 0x64;
        obj->field_252 = 0;
    }
    func_801307e0(obj, n);
    if (obj->field_7e == 0) {
        func_80157380(obj);
        func_801b3654_slot04_14(obj);
    }
}

void func_801b34e8_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        play_side_sound_at(obj, obj->side, 0xc);
        func_801b3564_slot04_14(obj);
    } else {
        obj->pos_x = obj->pos_x + (s8)*(u8 *)&obj->step_flags;
        update_sequence(obj);
    }
}

void func_801b3564_slot04_14(Object *obj) {
    Object *other = obj->other;
    int t;

    if (obj->variant_index != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    obj->facing_side = obj->saved_facing;
    if (((obj->bound_push_flag >> obj->variant_index) & 1) == 0) {
        t = 0x30000;
        if (obj->input_dir_second != 0) {
            t = 0x24000;
        }
        if (t < obj->vel_x) {
            update_sequence(obj);
            return;
        }
    }
    obj->facing_side = obj->saved_facing;
    other->other_state_code = 5;
    func_801312b8(obj);
    if (obj->field_7e == 0) {
        func_8013788c(obj);
    }
}

void func_801b3654_slot04_14(Object *obj) {
    if (obj->field_49 == 0) {
        func_801379bc(obj, 1);
    }
}

void func_801b3684_slot04_14(Object *obj) {
    data_801c6378_slot04_14[obj->state_3](obj);
}

void func_801b36c4_slot04_14(Object *obj) {
    obj->field_225 = 1;
    obj->state_3 = 1;
    if (obj->field_7e == 0) {
        func_80145d20(obj);
    }
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x3c);
}

void func_801b3730_slot04_14(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    Object *other = obj->other;
    s16 t;
    int i;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        ((Slot04bObj *)obj)->table_value = 1;
        obj->field_165 = 0;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_4b != 0) {
            other->field_6b = 0xa;
            ((Slot04bObj *)obj)->table_value = 0;
        }
    } else if ((t & 0xff) != 0) {
        if (obj->field_165 == 0) {
            obj->field_165 = 0xff;
            if (obj->field_4b != 0) {
                obj->field_165 = 1;
            }
        }
        play_sound_at(obj, obj->side, 0x31c);
        i = (((u8)obj->step_flags - 1) << 2) & 0x1fc;
        obj->step_flags = obj->step_flags & 0xff00;
        func_801482e0(obj, *(s16 *)((u8 *)data_801c6390_slot04_14 + i), *(s16 *)((u8 *)data_801c6390_slot04_14 + i + 2));
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b382c_slot04_14(Object *obj) {
    Object *p;
    u16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 1) {
        obj->state_3 = obj->state_3 + 1;
        obj->countdown = *(u8 *)&obj->countdown | 0x800;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x14;
            p->table_index = 8;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->vel_table_index = (obj->direction_index >> 1) + 0xc;
            p->health = obj->direction_index + 3;
            p->active_flag_2 = 1;
            p->facing_side = obj->facing_side;
            p->field_0e = obj->field_0e;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            y = obj->ground_y;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->ground_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags = obj->link_flags + 1;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}

void func_801b3998_slot04_14(Object *obj) {
    s16 t = obj->countdown;
    int n;

    if ((t & 0xff00) != 0) {
        t = t - 0x100;
        obj->countdown = t;
        if ((t & 0xff00) == 0) {
            obj->vel_y = 0x74000;
            obj->accel_y = -0x5000;
            obj->vel_x = 0x10000;
            obj->accel_x = -0x280;
            if (obj->facing_side != 0) {
                obj->vel_x = -0x10000;
                obj->accel_x = 0x280;
            }
        } else {
            update_sequence(obj);
        }
    } else if ((u8)func_80130184(obj) != 0) {
        if ((s16)obj->step_flags < 0) {
            obj->active_flag_3 = 0;
            obj->state_3 = obj->state_3 + 1;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        n = 0x39;
        if (obj->variant_index == 1) {
            n = 0x38;
        }
        func_801307e0(obj, n);
    }
}

void func_801b3ab0_slot04_14(Object *obj) {
    int n;

    if ((u8)func_80130184(obj) == 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        n = 0x39;
        if (obj->variant_index == 1) {
            n = 0x38;
        }
        func_801307e0(obj, n);
    } else {
        update_sequence(obj);
    }
}

void func_801b3b2c_slot04_14(Object *obj) {
    data_801c6398_slot04_14[obj->state_3](obj);
}

void func_801b3b6c_slot04_14(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    func_801307e0(obj, obj->field_49 != 0 ? 0x68 : 0x37);
}

void func_801b3bd0_slot04_14(Object *obj) {
    u16 n = 0x32;

    if ((s16)obj->step_flags < 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_49 != 0) {
            n = 0x69;
        }
        n = (obj->direction_index >> 1) + n;
        func_801307e0(obj, n);
    } else {
        update_sequence(obj);
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b3c38_slot04_14(Object *obj) {
    Object *p;
    u16 y;
    int x;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->countdown = *(u8 *)&obj->countdown | 0x600;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x14;
            p->table_index = 4;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            x = obj->direction_index >> 1;
            p->active_flag_2 = 0;
            p->vel_table_index = x + 6;
            p->health = x;
            p->facing_side = obj->facing_side;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            y = obj->ground_y;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->ground_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags = obj->link_flags + 1;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}

void func_801b3d78_slot04_14(Object *obj) {
    data_801c63a8_slot04_14[obj->state_3](obj);
}
