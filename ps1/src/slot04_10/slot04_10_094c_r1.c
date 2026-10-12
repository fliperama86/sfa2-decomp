/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6988_slot04_10[];

void set_sequence_by_side(Object *object, int arg);
int func_80130184(Object *object);

void func_801b0bdc_slot04_10(Object *obj);
void func_801b0c38_slot04_10(Object *obj);
void func_801b0c8c_slot04_10(Object *obj);
void func_801b0ce8_slot04_10(Object *obj);
void func_801b10b8_slot04_10(Object *obj);

void func_801b094c_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        func_801209c4(obj);
        func_801b10b8_slot04_10(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b09c0_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        func_801209c4(obj);
        func_801b10b8_slot04_10(obj);
        if (*(u32 *)&obj->state_0 == 0x2030001) {
            func_801307e0(obj, 0x43);
        }
    } else {
        update_sequence(obj);
    }
}

void func_801b0a50_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        func_801209c4(obj);
        func_801b10b8_slot04_10(obj);
        if (*(u32 *)&obj->state_0 == 0x2030001) {
            func_801307e0(obj, 0x44);
        }
    } else {
        update_sequence(obj);
    }
}

void func_801b0ae0_slot04_10(Object *obj) {
    u16 t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    t = obj->input_bits_2;
    if (obj->variant_index == 0) {
        if ((t & 0x1000) && obj->input_dir_second == 0 && obj->direction_index != 0) {
            func_801b0ce8_slot04_10(obj);
        } else {
            func_801b0bdc_slot04_10(obj);
        }
    } else if ((t & 0x4000) == 0) {
        func_801b0bdc_slot04_10(obj);
    } else if (obj->input_dir_second == 0) {
        if (obj->direction_index == 4) {
            func_801b0c38_slot04_10(obj);
        } else {
            func_801b0bdc_slot04_10(obj);
        }
    } else if (obj->direction_index == 4) {
        func_801b0bdc_slot04_10(obj);
    } else {
        func_801b0c8c_slot04_10(obj);
    }
}

void func_801b0bdc_slot04_10(Object *obj) {
    s16 a = 0xc;

    if (obj->variant_index != 0) {
        a = 0x12;
    }
    if (obj->input_dir_second != 0) {
        a += 3;
    }
    a += obj->direction_index >> 1;
    func_801307e0(obj, a);
}

void func_801b0c38_slot04_10(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    obj->step_limit = 0;
    func_80141f28(obj, 1);
    func_801307e0(obj, 0x4c);
}

void func_801b0c8c_slot04_10(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    obj->step_limit = 1;
    func_80141f28(obj, 1);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x4d);
}

void func_801b0ce8_slot04_10(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    obj->step_limit = 2;
    func_80141f28(obj, 1);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x4e);
}

void func_801b0d48_slot04_10(Object *obj) {
    data_801c6988_slot04_10[obj->state_2](obj);
}

void func_801b0d88_slot04_10(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void func_801b0dbc_slot04_10(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void func_801b0e14_slot04_10(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 tbl[16] = { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
    int v;

    o->state_2 = o->state_2 + 1;
    o->countdown = 0x50;
    if (game_state.config->countdown_2 == 0) {
        game_state.config->countdown_2 = 0x1e;
    }
    v = (u8)func_80125734(o, tbl[get_random_byte() & 0xf]);
    obj->saved_pos_x_frac = v;
    set_sequence_by_side(o, v + 0x23);
}

