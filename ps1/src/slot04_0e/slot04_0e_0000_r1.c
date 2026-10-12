/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80079a38_slot2b[];
extern u8 data_801b6f60_slot04_0e[];
extern u32 data_801c5dc4_slot04_0e[];
extern ObjectFn data_801c5e64_slot04_0e[];
extern ObjectFn data_801c5e70_slot04_0e[];
extern s8 data_801c5e7c_slot04_0e[];
extern u16 data_801c5e8c_slot04_0e[];
extern ObjectFn data_801c5e98_slot04_0e[];
extern Slot04_0eRec5eb0 data_801c5eb0_slot04_0e[];
extern ObjectFn data_801c5ee0_slot04_0e[];

Block172 *func_8011f1e0(void);
Object *func_8011f32c(void);
void func_80152df4(Object *object);
void set_sequence_by_side(Object *object, int arg);

int func_801b0a68_slot04_0e(Object *obj, Object *other);
void func_801b0a94_slot04_0e(Object *obj, int a);
void func_801b0b10_slot04_0e(Object *obj);
void func_801b0bb8_slot04_0e(Object *obj);
void func_801b0bd8_slot04_0e(Object *obj);
void func_801b0cd0_slot04_0e(Object *obj);
void func_801b6808_slot04_0e(Object *obj);
void func_801b687c_slot04_0e(Object *obj);

void func_801b0000_slot04_0e(Object *obj) {
    u32 *dst;
    int i;
    Object *c;

    obj->data_ptr_a = data_80079a38_slot2b;
    obj->data_ptr_b = data_801b6f60_slot04_0e;
    dst = (u32 *)0x1f800100;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c5dc4_slot04_0e[i];
    }
    if (game_state.count_index == 0) {
        *(s32 *)&obj->pos_y_frac = obj->ground_y + 0xf0;
        ((Slot04bObj *)obj)->field_1da = 0;
    } else {
        ((Slot04bObj *)obj)->field_1da = 0;
    }
    c = (Object *)func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0xe;
        c->parent = obj;
        ((Slot04bObj *)obj)->field_2c = (u32)c;
        *(s32 *)&c->pos_x_frac = *(s32 *)&obj->pos_x_frac;
        *(s32 *)&c->pos_y_frac = *(s32 *)&obj->pos_y_frac;
        c->facing_side = obj->facing_side;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->table_index = obj->kind;
        c->sound_side = obj->side;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}

void func_801b015c_slot04_0e(Object *obj) {
    data_801c5e64_slot04_0e[obj->state_2](obj);
}

void func_801b019c_slot04_0e(Object *obj) {
    if (((Slot04bObj *)obj)->mode == 0) {
        obj->active_state = 1;
        obj->vel_y = 0;
        obj->accel_y = -0x2000;
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void func_801b01e8_slot04_0e(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->active_state = 0;
        obj->vel_y = 0;
        obj->accel_y = 0;
        obj->pos_y_frac = 0;
        obj->state_2 = obj->state_2 + 1;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        func_801209c4(obj);
        set_sequence_by_side(obj, 0x11);
    }
}

void func_801b0284_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    }
}

void func_801b02d4_slot04_0e(Object *obj) {
    data_801c5e70_slot04_0e[obj->state_2](obj);
}

void func_801b0314_slot04_0e(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void func_801b0348_slot04_0e(Object *obj) {
    u16 a;

    if (game_state.field_64 == 0 && game_state.countdown_timer == 0) {
        obj->state_2 = obj->state_2 + 1;
        obj->state_3 = 0;
        ((Slot04bObj *)obj)->field_47 = 0x3c;
        game_state.countdown = 0x1e;
        a = (s8)func_80125734(obj, (s8)data_801c5e7c_slot04_0e[get_random_byte() & 0xf]);
        if ((game_state.field_4d | game_state.field_04) != 0) {
            if (a == 1) {
                a = 5;
            }
        }
        obj->variant_index = a;
        set_sequence_by_side(obj, data_801c5e8c_slot04_0e[a]);
    } else {
        update_sequence(obj);
    }
}

void func_801b043c_slot04_0e(Object *obj) {
    u8 t;

    if (((Slot04bObj *)obj)->field_47 != 0) {
        t = ((Slot04bObj *)obj)->field_47 - 1;
        ((Slot04bObj *)obj)->field_47 = t;
        if (t == 0) {
            game_state_second.side_bits = game_state.side_bits_2 | (1 << obj->side);
        }
    }
    data_801c5e98_slot04_0e[obj->variant_index](obj);
}

void func_801b04c8_slot04_0e(Object *obj) {
    Object *c;
    u8 *r;
    u16 x;

    if (((Slot04bObj *)obj)->mode == 1) {
        c = (Object *)func_8011f1e0();
        if (c != 0) {
            c->active_count = 1;
            c->field_02 = 0x14;
            c->ot_depth = 0;
            c->field_0e = 0;
            c->pos_y = -((Slot06Layer *)data_801aa5d4)->scroll_y;
            c->parent = obj;
            r = (u8 *)data_801c5eb0_slot04_0e + ((((Slot04bObj *)obj)->mode_bits & 0xf) << 2);
            x = *(u16 *)r;
            c->pos_x = x;
            r += 2;
            c->pos_x = x + ((Slot06Layer *)data_801aa5d4)->scroll_x;
            c->table_index = r[0];
            c->variant_index = r[1];
            ((Slot04bObj *)obj)->mode = 0;
            c->spawn_x = 0x60;
            c->spawn_y = 0x1e0;
            c->image_row = obj->image_row;
            c->mode_id = 0x20;
            c->field_90 = obj->field_90;
            c->sound_side = obj->sound_side;
            c->data_ptr_a = obj->data_ptr_a;
            c->data_ptr_b = obj->data_ptr_b;
        }
    } else if (((Slot04bObj *)obj)->mode == 2) {
        ((Slot04bObj *)obj)->mode = 0;
        func_801b0cd0_slot04_0e(obj);
    } else if (((Slot04bObj *)obj)->mode == 3) {
        ((Slot04bObj *)obj)->mode = 0;
        play_side_sound_at(obj, obj->field_02, 0x15);
    }
    update_sequence(obj);
}

void func_801b0618_slot04_0e(Object *obj) {
    data_801c5ee0_slot04_0e[obj->state_3](obj);
    update_sequence(obj);
}

void func_801b066c_slot04_0e(Object *obj) {
    Object *c;
    int x;

    if (((Slot04bObj *)obj)->mode == 1) {
        c = (Object *)func_8011f1e0();
        if (c != 0) {
            c->active_count = 1;
            c->field_02 = 0x3b;
            c->table_index = 2;
            c->ot_depth = 0;
            c->field_02 = 0x14;
            c->team_id = obj->team_id;
            c->parent = obj;
            c->facing_side = obj->facing_side;
            x = 0x33;
            if (obj->facing_side == 0) {
                x = -0x33;
            }
            c->pos_x = x + (u16)obj->pos_x;
            c->pos_y = ((Slot04bObj *)obj)->ground_y - 0x3b;
            ((Slot04bObj *)obj)->mode = 0;
            obj->state_3 = obj->state_3 + 1;
            c->spawn_x = 0x60;
            c->spawn_y = 0x1e0;
            c->image_row = obj->image_row;
            c->mode_id = 0x20;
            c->field_90 = obj->field_90;
            c->sound_side = obj->sound_side;
            c->data_ptr_a = obj->data_ptr_a;
            c->data_ptr_b = obj->data_ptr_b;
        }
    }
}

void func_801b0780_slot04_0e(Object *obj) {
    if (((Slot04bObj *)obj)->mode == 2) {
        ((Slot04bObj *)obj)->mode = 0;
        obj->state_3 = obj->state_3 + 1;
        func_801b6808_slot04_0e(obj);
    }
}

void func_801b07bc_slot04_0e(Object *obj) {
    Object *o;

    if (((Slot04bObj *)obj)->mode == 3) {
        ((Slot04bObj *)obj)->mode = 0;
        obj->state_3++;
        o = obj->other;
        func_80152df4(o);
        play_sound_at(o, o->field_02, 0x321);
        obj->vel_y = 0x5c000;
        obj->accel_y = -0x5400;
        o->active_state = 1;
        func_801b0a94_slot04_0e(o, 0x13);
        func_801b0bd8_slot04_0e(o);
    }
}

void func_801b0854_slot04_0e(Object *obj) {
    Object *o = obj->other;

    func_801b0a68_slot04_0e(obj, o);
    if (o->ground_y >= o->pos_y) {
        func_801b0b10_slot04_0e(o);
        func_801b0bd8_slot04_0e(o);
    } else {
        obj->state_3 = obj->state_3 + 1;
        o->active_state = 0;
        o->pos_y_frac = 0;
        o->pos_y = ((Slot04bObj *)o)->ground_y;
        ((Slot04bObj *)obj)->countdown = 4;
        func_801b0a94_slot04_0e(o, 0x14);
        func_801b0bb8_slot04_0e(o);
        func_801b687c_slot04_0e(obj);
    }
}

void func_801b08fc_slot04_0e(Object *obj) {
    Object *o;

    ((Slot04bObj *)obj)->countdown--;
    o = obj->other;
    if (((Slot04bObj *)obj)->countdown == 0) {
        obj->state_3 = obj->state_3 + 1;
        o->active_state = 1;
        obj->vel_y = 0x28000;
        obj->accel_y = -0x4800;
        func_801b0a94_slot04_0e(o, 0x15);
    } else {
        func_801b0b10_slot04_0e(o);
    }
}

void func_801b0974_slot04_0e(Object *obj) {
    Object *o = obj->other;

    func_801b0a68_slot04_0e(obj, o);
    if (o->ground_y >= o->pos_y) {
        func_801b0b10_slot04_0e(o);
    } else {
        obj->state_3 = obj->state_3 + 1;
        o->active_state = 0;
        o->pos_y_frac = 0;
        o->pos_y = ((Slot04bObj *)o)->ground_y;
        func_801b0a94_slot04_0e(o, 0x16);
    }
}

void func_801b09fc_slot04_0e(Object *obj) {
    func_801b0b10_slot04_0e(obj->other);
}
