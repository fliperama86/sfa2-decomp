/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void set_sequence_by_side(Object *object, int index);
void func_80146998(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b3b74_slot04_15(Object *object);
void func_801b1548_slot04_15(Object *obj);
void func_801b1a00_slot04_15(Object *object);
void func_801b2020_slot04_15(Object *object);

extern u8 data_801c2df4_slot04_15[];
extern ObjectFn data_801c309c_slot04_15[];
extern ObjectFn data_801c30ac_slot04_15[];
extern ObjectFn data_801c30bc_slot04_15[];
extern ObjectFn data_801c30c4_slot04_15[];

void func_801b1548_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;
    int a = 0x2b;

    o->state_3++;
    t = data_801c2df4_slot04_15[obj->table_index_3];
    o->field_225 = 1;
    o->direction_index = t * 2;
    if (o->field_49 != 0) {
        a = 0x49;
    }
    a += t;
    func_801307e0(o, (u8)a);
}

void func_801b15ac_slot04_15(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        ((Slot04bObj *)obj)->countdown = 5;
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x1f;
            p->table_index = 0;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->health = 0;
            p->pos_y = y;
            p->parent = obj;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            if (obj->direction_index == 0) {
                play_side_sound_at(obj, obj->side, 0x13);
            } else {
                play_side_sound_at(obj, obj->side, 0x14);
            }
        }
    }
}

void func_801b1700_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->step_flags < 0) {
        func_801312b8(o);
    } else {
        if (obj->countdown != 0) {
            obj->countdown--;
            if (obj->countdown != 0) {
                goto tail;
            }
            o->active_flag_4 = 0;
        }
        func_80142adc(o);
    tail:
        update_sequence(o);
    }
}

void func_801b1788_slot04_15(Object *obj) {
    data_801c309c_slot04_15[obj->state_3](obj);
}

void func_801b17c8_slot04_15(Object *obj) {
    int a;

    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    a = 0x2f;
    if (obj->active_state == 0) {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void func_801b1834_slot04_15(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state == 0) {
            a = 1;
            b = 0x49;
        } else {
            a = -5;
            b = 0x44;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3b74_slot04_15(obj);
    }
    update_sequence(obj);
}

void func_801b18d8_slot04_15(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b1964_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->counter++;
    update_sequence(o);
    o->countdown = t = (o->countdown & 0xff00) | ((o->countdown & 0xff) - 1);
    if ((t & 0xff) != 0) {
        func_801b1a00_slot04_15(o);
    } else {
        if (o->active_state != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    }
}

void func_801b1a00_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_134 != 0) {
        if (obj->saved_count == 0) {
            obj->saved_count = obj->counter;
        }
    } else if (*(u8 *)&o->countdown != 0) {
        (*(u8 *)&o->countdown)--;
    }
    func_80142fe8(o);
}

void func_801b1a6c_slot04_15(Object *obj) {
    data_801c30ac_slot04_15[obj->state_3](obj);
}

void func_801b1aac_slot04_15(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801307e0(obj, 0x2f);
    }
}

void func_801b1b1c_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    if (obj->mode != 0) {
        o->active_state = 1;
        o->state_3++;
        o->vel_x = 0x18000;
        o->vel_y = 0xfff88000;
        o->accel_y = 0x6000;
    }
}

void func_801b1b84_slot04_15(Object *obj) {
    int d;

    func_801b2020_slot04_15(obj);
    if (obj->ground_y > obj->pos_y) {
        d = obj->vel_x;
        if (obj->facing_side == 0) {
            d = -d;
        }
        *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801307e0(obj, 0x26);
    }
}

void func_801b1c20_slot04_15(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->other->other_state_code = 5;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1c64_slot04_15(Object *obj) {
    data_801c30bc_slot04_15[obj->state_3](obj);
}

void func_801b1ca4_slot04_15(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801307e0(obj, 0x30);
    }
}

void func_801b1d14_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b1d54_slot04_15(Object *obj) {
    data_801c30c4_slot04_15[obj->state_3](obj);
}
