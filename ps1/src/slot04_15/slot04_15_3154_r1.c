/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2eac_slot04_15[];
extern u8 data_801c2ecc_slot04_15[];
extern ObjectFn data_801c314c_slot04_15[];
extern ObjectFn data_801c3160_slot04_15[];
extern ObjectFn data_801c3170_slot04_15[];

Block172 *func_8011f1e0(void);
void set_sequence_by_side(Object *object, int arg);
void set_position_from_other_box(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80131468(Object *object);
void func_801b36cc_slot04_15(Object *obj);
void func_801b39d0_slot04_15(Object *object, GameState *g);

void func_801b3154_slot04_15(Object *obj) {
    Object *o;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
        set_position_from_other_box(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        func_80146478(obj, 1, 0x18, 0x47);
        o = obj->other;
        ref_other.p = o;
        play_sound_at(o, ref_other.p->side, 0x306);
        func_801307e0(obj, 0x1a);
    }
}

void func_801b31f0_slot04_15(Object *obj) {
    Object *o;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->vel_x = 0;
        obj->vel_y = 0;
        obj->accel_y = 0x6000;
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0x1e, 0xf, 1, 0, 1, 1);
        o = obj->other;
        if (o->field_15b != 0) {
            o->field_243 = 0;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}

void func_801b328c_slot04_15(Object *obj) {
    func_801b36cc_slot04_15(obj);
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801307e0(obj, 0x1b);
    }
}

void func_801b3304_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        func_801312b8(obj);
    }
}

void func_801b3350_slot04_15(Object *obj) {
    s16 y;

    func_801b36cc_slot04_15(obj);
    if (((Slot04bObj *)obj)->saved_pos_x <= 0 || obj->pos_y < (y = obj->ground_y)) {
        update_sequence(obj);
        update_sequence(obj);
    } else {
        obj->pos_y = y;
        obj->active_state = 0;
        func_801312b8(obj);
    }
}

void func_801b33c8_slot04_15(Object *obj) {
    data_801c314c_slot04_15[obj->state_3](obj);
}

void func_801b3408_slot04_15(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    func_801307e0(obj, 0x20);
    set_position_from_other_box(obj);
}

void func_801b3484_slot04_15(Object *obj) {
    Object *o;
    int x;

    update_sequence(obj);
    set_position_from_other_box(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        func_80146478(obj, 2, -0x49, 0x30);
        x = 0x36000;
        if (obj->facing_side != 0) {
            x = -0x36000;
        }
        obj->vel_y = -0x40000;
        obj->vel_x = x;
        obj->accel_y = 0x6000;
        func_80140770(obj, 0x1d, 5, 0xf, 0, 0, 0);
        o = obj->other;
        ref_other.p = o;
        if ((s16)obj->health >= 0) {
            play_sound_at(o, ref_other.p->side, 0x30f);
        } else {
            play_sound_at(o, ref_other.p->side, 0x346);
        }
    }
}

void func_801b3598_slot04_15(Object *obj) {
    func_801b36cc_slot04_15(obj);
    if (((Slot04bObj *)obj)->saved_pos_x < 0 || (s16)(obj->ground_y - 0x50) >= obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        func_801307e0(obj, 0x21);
    }
}

void func_801b3614_slot04_15(Object *obj) {
    func_801b36cc_slot04_15(obj);
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801307e0(obj, 0x22);
    }
}

void func_801b368c_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b36cc_slot04_15(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void func_801b3700_slot04_15(Object *obj) {
    func_801312b8(obj);
}

void func_801b3720_slot04_15(Object *obj) {
    func_80131468(obj);
}

void func_801b3740_slot04_15(Object *obj) {
    data_801c3160_slot04_15[obj->state_2](obj);
}

void func_801b3780_slot04_15(Object *object) {
    object->state_2 = object->state_2 + 1;
    object->facing_side = object->saved_facing;
    set_sequence_by_side(object, 0);
}

void func_801b37b4_slot04_15(Object *object) {
    if (game_state.countdown_timer == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void func_801b37f0_slot04_15(Object *object) {
    u16 a;
    ((Slot04bObj *)object)->countdown = 0x3c;
    object->state_2 = object->state_2 + 1;
    if (game_state.countdown == 0) {
        game_state.countdown = 0x1e;
    }
    a = object->input_bits & 0xf000;
    if (((Slot04bObj *)object)->field_5c >= 0x90 && a == 0x1000) {
        a = 1;
    } else {
        a = get_random_byte();
        a &= 0x1f;
        if (((Slot04bObj *)object)->field_5c < 0x80) {
            a = data_801c2eac_slot04_15[a];
        } else {
            a = data_801c2ecc_slot04_15[a];
        }
    }
    a = (s8)func_80125734(object, (s8)a); object->substate_index = a; a += 0x23; set_sequence_by_side(object, (s16)a);
}

void func_801b38e8_slot04_15(Object *object) {
    data_801c3170_slot04_15[object->substate_index](object);
}

void func_801b3928_slot04_15(Object *object) {
    int a;
    s8 r;

    if ((s16)object->step_flags < 0) {
        r = get_random_byte();
        a = r;
        if ((a & 3) != 0) {
            a = 0x32;
        } else {
            a = 0x33;
        }
        set_sequence_by_side(object, (u8)a);
    }
    func_801b39d0_slot04_15(object, &game_state);
    update_sequence(object);
}

void func_801b3998_slot04_15(Object *object) {
    func_801b39d0_slot04_15(object, &game_state);
    update_sequence(object);
}

void func_801b39d0_slot04_15(Object *object, GameState *g) {
    if (((Slot04bObj *)object)->countdown != 0) {
        ((Slot04bObj *)object)->countdown--;
        if (((Slot04bObj *)object)->countdown == 0) {
            g->side_bits_2 |= 1 << object->side;
        }
    }
}

void func_801b3a1c_slot04_15(Object *object) {
    Object *p;
    int dx;
    u16 e;
    u8 d;

    update_sequence(object);
    if (object->step_flags & 0xff) {
        object->step_flags = object->step_flags & 0xff00;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x77;
            p->parent = object;
            p->field_0c = object->field_0c;
            p->image_row = object->image_row;
            p->field_0e = object->field_0e;
            p->facing_side = object->facing_side;
            p->field_1c = object->field_1c;
            e = object->cycle_value;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->cycle_value = e;
            d = object->image_row;
            p->field_02 = 0x17;
            p->mode_id = 0x20;
            p->image_row = d;
            p->field_90 = object->field_90;
            p->data_ptr_a = object->data_ptr_a;
            p->data_ptr_b = object->data_ptr_b;
            p->sound_side = object->sound_side;
            *(s32 *)&p->pos_x_frac = *(s32 *)&object->pos_x_frac;
            *(s32 *)&p->pos_y_frac = *(s32 *)&object->pos_y_frac;
            p->pos_y = p->pos_y - 0x63;
            dx = -0x3b;
            if (object->facing_side != 0) {
                dx = 0x3b;
            }
            p->pos_x = dx + p->pos_x;
        }
    }
    func_801b39d0_slot04_15(object, &game_state);
}
