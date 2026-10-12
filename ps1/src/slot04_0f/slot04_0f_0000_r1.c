/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6e9c_slot04_0f[];
extern u8 data_801b9aa8_slot04_0f[];
extern u32 data_801c58c0_slot04_0f[];
extern ObjectFn data_801c5960_slot04_0f[];
extern ObjectFn data_801c5968_slot04_0f[];
extern ObjectFn data_801c5974_slot04_0f[];
extern ObjectFn data_801c5980_slot04_0f[];
extern u8 data_801c5994_slot04_0f;
extern u8 data_801c5998_slot04_0f;
extern u8 data_801c599c_slot04_0f;
extern u8 data_801c59a0_slot04_0f;
extern ObjectFn data_801c59a4_slot04_0f[];
extern u8 data_801c59b0_slot04_0f[];
extern u8 data_801c59b4_slot04_0f;
extern ObjectFn data_801c59b8_slot04_0f[];
extern u8 data_801c59c4_slot04_0f[];
extern u8 data_801c59c8_slot04_0f[];
extern ObjectFn data_801c59cc_slot04_0f[];
extern u8 data_801c59d8_slot04_0f[];
extern u8 data_801c59dc_slot04_0f[];
extern ObjectFn data_801c59e0_slot04_0f[];

Block172 *func_8011f1e0(void);
void set_sequence_by_side(Object *object, int index);
void func_80130dc0(Object *object);
void func_80142a14(Object *object);
void func_801e9ed0_slot06_0f(void);

void func_801b02c8_slot04_0f(Object *obj);
void func_801b06fc_slot04_0f(Object *obj);
void func_801b09a4_slot04_0f(Object *obj);
void func_801b22ec_slot04_0f(Object *obj);
void func_801b2270_slot04_0f(Object *obj);
void func_801b4a3c_slot04_0f(Object *obj);

void func_801b0000_slot04_0f(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b6e9c_slot04_0f;
    obj->data_ptr_b = data_801b9aa8_slot04_0f;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c58c0_slot04_0f[i];
    }
    if (game_state.config->compare_value == 0xf) {
        if (obj->other->kind != 0xf || obj->side == 0) {
            func_801e9ed0_slot06_0f();
        }
    }
}

void func_801b00bc_slot04_0f(Object *obj) {
    data_801c5960_slot04_0f[obj->state_2](obj);
}

void func_801b00fc_slot04_0f(Object *obj) {
    Object *c;

    obj->state_2 = obj->state_2 + 1;
    c = (Object *)func_8011f1e0();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x21;
        c->table_index = 0;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        c->pos_x = obj->pos_x;
        c->pos_y = obj->pos_y;
        c->facing_side = obj->facing_side;
        c->ot_depth = 6;
        c->field_1c = obj->field_1c;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->image_row = obj->image_row;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->mode_id = 0x20;
        c->field_02 = 2;
        c->sound_side = obj->sound_side;
    }
}

void func_801b01f4_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    } else {
        update_sequence(obj);
    }
}

void func_801b0244_slot04_0f(Object *obj) {
    data_801c5968_slot04_0f[obj->input_dir_0 >> 1](obj);
}

void func_801b0288_slot04_0f(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b06fc_slot04_0f(obj);
    } else {
        func_801b02c8_slot04_0f(obj);
    }
}

void func_801b02c8_slot04_0f(Object *obj) {
    data_801c5974_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b030c_slot04_0f(Object *obj) {
    data_801c5980_slot04_0f[obj->state_3](obj);
}

void func_801b034c_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 *p;
    u16 t;

    o->active_flag_3 = 1;
    t = o->input_bits_2;
    obj->field_337 = 0;
    o->state_3 = o->state_3 + 1;
    p = &obj->saved_pos_x_frac;
    if ((t & 0x8000) != 0) {
        func_80130dc0(o);
    } else if ((t & 0x2000) != 0 && (t & 0x80) != 0) {
        o->state_3 = 3;
        obj->countdown_timer = 0x21;
        func_801307e0(o, 0x60);
    } else {
        o->state_3 = o->state_3 + 1;
        p[7] = 0xff;
        func_80130ec0(o);
        o->kind_table_value = data_801c5994_slot04_0f;
        o->countdown_5 = data_801c5998_slot04_0f;
        func_801307e0(o, 0x1a);
    }
}

void func_801b0418_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b0438_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b0458_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->countdown_timer--;
    if (obj->countdown_timer == 0) {
        o->state_3 = o->state_3 + 1;
        func_801307e0(o, 0x61);
        obj->field_339 = 0;
        o->sequence_hold_flag = 1;
        o->step_flags = o->step_flags & 0xff00;
        func_801b2270_slot04_0f(o);
    } else if ((o->input_bits_2 & 0x80) == 0) {
        o->state_3 = o->state_3 + 1;
        o->kind_table_value = data_801c599c_slot04_0f;
        o->countdown_5 = data_801c59a0_slot04_0f;
        func_801307e0(o, 0x1a);
    } else {
        update_sequence(o);
    }
}

void func_801b0530_slot04_0f(Object *obj) {
    if ((u8)obj->step_flags != 1) {
        obj->state_3 = 2;
        func_801b22ec_slot04_0f(obj);
    } else {
        func_801b4a3c_slot04_0f(obj);
    }
}

void func_801b0570_slot04_0f(Object *obj) {
    data_801c59a4_slot04_0f[obj->state_3](obj);
}

void func_801b05b0_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 *p;
    int k;

    o->state_3 = o->state_3 + 1;
    p = &obj->saved_pos_x_frac;
    obj->field_337 = 0;
    if (o->direction_index != 0 && (o->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(o, -0x14, 0x14) != 0) {
        o->state_0 = 1;
        o->state_1 = 2;
        o->state_2 = 0;
        o->state_3 = 0;
    } else {
        o->active_flag_3 = 1;
        if ((o->input_bits_2 & 0x2000) == 0) {
            func_80130dc0(o);
        } else {
            o->state_3 = o->state_3 + 1;
            func_80130ec0(o);
            p[7] = 0xff;
            k = o->direction_index >> 1;
            func_80141f28(o, k);
            o->kind_table_value = data_801c59b0_slot04_0f[k];
            o->countdown_5 = data_801c59b4_slot04_0f;
            func_801307e0(o, k + 0x1a);
        }
    }
}

void func_801b06bc_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b06dc_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b06fc_slot04_0f(Object *obj) {
    data_801c59b8_slot04_0f[obj->state_3](obj);
}

void func_801b073c_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int k;

    o->active_flag_3 = 1;
    o->state_3 = o->state_3 + 1;
    obj->field_337 = 0;
    if ((o->input_bits_2 & 0x2000) == 0) {
        func_80130dc0(o);
    } else {
        o->state_3 = o->state_3 + 1;
        func_80130ec0(o);
        k = o->direction_index >> 1;
        obj->field_337 = 0xff;
        func_80141f28(o, k);
        o->kind_table_value = data_801c59c4_slot04_0f[k];
        o->countdown_5 = data_801c59c8_slot04_0f[k];
        func_801307e0(o, k + 0x1d);
    }
}

void func_801b0800_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b0820_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b0840_slot04_0f(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second == 0) {
        data_801c59cc_slot04_0f[obj->state_3](obj);
    } else {
        func_801b09a4_slot04_0f(obj);
    }
}

void func_801b08a0_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int k;

    o->active_flag_3 = 1;
    o->state_3 = o->state_3 + 1;
    obj->field_337 = 0;
    if ((o->input_bits_2 & 0x2000) == 0) {
        func_80130dc0(o);
    } else {
        obj->field_337 = 0xff;
        o->state_3 = o->state_3 + 1;
        func_80130ec0(o);
        k = o->direction_index >> 1;
        func_80141f28(o, k);
        o->kind_table_value = data_801c59d8_slot04_0f[k];
        o->countdown_5 = data_801c59dc_slot04_0f[k];
        func_801307e0(o, k + 0x20);
    }
}

void func_801b0964_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b0984_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b09a4_slot04_0f(Object *obj) {
    data_801c59e0_slot04_0f[obj->state_3](obj);
}
