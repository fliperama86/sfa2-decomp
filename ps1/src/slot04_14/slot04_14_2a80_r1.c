/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6324_slot04_14[];
extern ObjectFn data_801c6334_slot04_14[];
extern s32 data_801c6350_slot04_14[];
extern s32 data_801c6354_slot04_14[];
extern u8 data_801c6368_slot04_14[];
extern ObjectFn data_801c636c_slot04_14[];
int func_80130184(Object *object);
void set_sequence_by_side(Object *object, int index);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_80146998(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801482e0(Object *object, int a, int b);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b2b4c_slot04_14(Object *object);

void func_801b2a80_slot04_14(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->active_flag_3 = 0;
            obj->state_3++;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        func_801b2b4c_slot04_14(obj);
    }
}

void func_801b2b00_slot04_14(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        func_801b2b4c_slot04_14(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b2b4c_slot04_14(Object *obj) {
    obj->active_flag_3 = 0;
    obj->active_state = 0;
    obj->pos_y = obj->ground_y;
    func_801b62a8_slot04_14(obj, 1, 0, 3, 2);
    set_sequence_by_side(obj, 0x11);
}

void func_801b2ba4_slot04_14(Object *obj) {
    data_801c6324_slot04_14[obj->state_3](obj);
}

void func_801b2be4_slot04_14(Object *obj) {
    obj->state_3++;
    obj->countdown = *(u8 *)&obj->countdown | 0x3200;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x28);
}

void func_801b2c48_slot04_14(Object *obj) {
    obj->countdown -= 0x100;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, 0x1e, 0x30);
        func_801482e0(obj, -0x2c, 0x30);
    }
    update_sequence(obj);
}

void func_801b2cdc_slot04_14(Object *obj) {
    int a;

    obj->countdown = (s16)obj->countdown - 0x100;
    if ((s16)obj->countdown < 0) {
        a = 0;
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            a = 6;
            obj->other->field_6b = 0xa;
        }
        obj->other_lock = a;
        func_801307e0(obj, 0x46);
    } else {
        update_sequence(obj);
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 5 instruction slots. */
void func_801b2d54_slot04_14(Object *obj) {
    Object *p;
    s16 t;
    u16 y;

    update_sequence(obj);
    t = obj->step_flags;
    if (!(t & 0x8000)) {
        if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x14;
            p->table_index = 6;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->vel_table_index = (obj->direction_index >> 1) + 9;
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
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0xb);
        }
        }
    } else {
        func_801312b8(obj);
    }
}

void func_801b2ecc_slot04_14(Object *obj) {
    s16 t;

    t = obj->step_flags;
    if (t < 0) {
        func_801312b8(obj);
    } else if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        func_80146998(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b2f28_slot04_14(Object *obj) {
    data_801c6334_slot04_14[obj->state_3](obj);
}

void func_801b2f68_slot04_14(Object *obj) {
    *(s32 *)&obj->vel_x = 0x80000;
    obj->accel_x = -0x8000;
    obj->state_3++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x2a);
}

void func_801b2fd8_slot04_14(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x12, 0x30);
    }
    update_sequence(obj);
}

void func_801b3054_slot04_14(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    u8 i = 0;
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        obj->vel_x = data_801c6350_slot04_14[obj->direction_index];
        obj->vel_y = data_801c6354_slot04_14[obj->direction_index];
    } else {
        if ((t & 0xff) == 2) {
            obj->step_flags = t & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        if (*(u8 *)&obj->step_flags == 0) {
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = (obj->direction_index >> 1) + 1;
                }
                obj->other_lock = data_801c6368_slot04_14[i];
            } else {
                if (obj->facing_side != 0) {
                    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
                } else {
                    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
                }
                obj->vel_x = obj->vel_x + obj->accel_x;
                if (obj->vel_x < 0) {
                    obj->accel_x = 0;
                    obj->vel_x = 0;
                }
            }
        }
    }
}

void func_801b31d0_slot04_14(Object *obj) {
    if (obj->vel_x >= 0) {
        if (*(u8 *)&obj->step_flags == 0) {
            obj->active_state = 1;
            *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
            obj->vel_y = obj->vel_y + obj->accel_y;
        }
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b3294_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b32ec_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        func_801209c4(obj);
    }
    update_sequence(obj);
}

void func_801b3384_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b33cc_slot04_14(Object *obj) {
    data_801c636c_slot04_14[obj->state_3](obj);
}
