/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c5f90_slot04_0e[];
extern ObjectFn data_801c5fc8_slot04_0e[];
extern ObjectFn data_801c6000_slot04_0e[];
extern s32 data_801c601c_slot04_0e[];
extern s32 data_801c6028_slot04_0e[];
extern ObjectFn data_801c6034_slot04_0e[];

void func_80142adc(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b2c00_slot04_0e(Object *obj);
void func_801b2ca4_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);
BytePair func_801b6b64_slot04_0e(Slot04bObj *obj);
void func_801b6d7c_slot04_0e(Slot04bObj *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);

void func_801b26a8_slot04_0e(Object *obj) {
    data_801ad398 = data_801c5f90_slot04_0e[obj->field_15a](obj);
}

int func_801b26f0_slot04_0e(Object *obj) {
    return obj->link_flags == 0;
}

int func_801b26fc_slot04_0e(Object *obj) {
    return 1;
}

int func_801b2704_slot04_0e(Object *obj) {
    return 0;
}

int func_801b270c_slot04_0e(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int func_801b2720_slot04_0e(Object *obj) {
    if (obj->field_7e == 0) {
        return obj->remaining_count != 0;
    }
    return 1;
}

int func_801b2744_slot04_0e(Object *obj) {
    return 0;
}

void func_801b274c_slot04_0e(Object *obj) {
    data_801c5fc8_slot04_0e[obj->field_15a](obj);
}

void func_801b278c_slot04_0e(Object *obj) {
    data_801c6000_slot04_0e[obj->state_3](obj);
}

void func_801b27cc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    o->active_flag_4 = 1;
    obj->state_flag = 0;
    o->vel_y = 0;
    o->accel_y = 0;
    o->state_3++;
    func_80141f28(o, 3);
    func_80138ae8(&game_state, o);
    func_801b6d7c_slot04_0e((Slot04bObj *)o);
    if (o->field_49 == 0) {
        a = 0x1d;
    } else {
        a = 0x58;
    }
    a += o->direction_index >> 1;
    func_801307e0(o, a);
}

void func_801b285c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    if (obj->state_flag == 0) {
        func_801b2ca4_slot04_0e(o);
    }
    if (obj->mode == 0) {
        o->active_state = 1;
        o->state_3++;
        obj->mode = 0;
        o->vel_x = 0;
        o->accel_x = 0;
        o->vel_y = data_801c601c_slot04_0e[o->direction_index >> 1];
        o->accel_y = data_801c6028_slot04_0e[o->direction_index >> 1];
    }
    update_sequence(o);
}

void func_801b2910_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    if (obj->state_flag == 0) {
        func_801b2ca4_slot04_0e(o);
    }
    func_801b4914_slot04_0e(o);
    if (o->ground_y < o->pos_y) {
        func_801b2c00_slot04_0e(o);
    } else if (o->pos_y >= (s16)(o->ground_y - 0x18) || obj->state_flag == 0) {
        update_sequence(o);
    } else {
        a = 0x5b;
        o->state_3++;
        if (o->field_49 == 0) {
            a = 0x52;
        }
        a += o->direction_index >> 1;
        func_801307e0(o, a);
    }
}

void func_801b29e0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    s16 y;

    if (obj->mode == 1) {
        p = pop_object_from_pool(o);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xe;
            p->table_index = 0;
            p->sound_side = o->sound_side;
            p->team_id = o->team_id;
            p->field_49 = o->field_49;
            p->vel_table_index = o->variant_index;
            p->active_flag_2 = 0;
            p->field_0e = o->field_0e;
            p->facing_side = o->facing_side;
            p->field_0c = o->field_0c;
            p->image_row = o->image_row;
            p->scroll_factor = o->scroll_factor;
            p->pos_x = o->pos_x;
            y = o->pos_y;
            p->health = 0;
            p->parent = o;
            p->pos_y = y;
            o->link_flags++;
            o->linked_object_3 = (s32)p;
            p->field_90 = o->field_90;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
        }
        obj->mode = 0;
        obj->field_47 = 2;
        o->state_3++;
        play_side_sound_at(o, o->field_02, 0x16);
        play_side_sound_at(o, o->field_02, 9);
    }
    update_sequence(o);
}

void func_801b2b40_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_47 -= 1;
    if (obj->field_47 & 0x80) {
        o->accel_y = -0x8000;
        o->step_timer = 1;
        o->state_3++;
        o->vel_y = 0;
        if (o->field_49 != 0) {
            o->accel_y = 0xfffe0000;
        }
        update_sequence(o);
    }
}

void func_801b2ba8_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->ground_y >= obj->pos_y) {
        update_sequence(obj);
    } else {
        func_801b2c00_slot04_0e(obj);
    }
}

void func_801b2c00_slot04_0e(Object *obj) {
    obj->state_3 = 6;
    obj->active_state = 0;
    obj->pos_y_frac = 0;
    obj->active_flag_4 = 0;
    obj->pos_y = (u16)obj->ground_y;
    func_801209c4(obj);
    func_801307e0(obj, 0x20);
}

void func_801b2c50_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2ca4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    BytePair buf;
    int t;

    if (o->mode_flag_2 != 0) {
        t = func_801b6bd4_slot04_0e(o);
    } else {
        t = obj->field_134;
    }
    if (t != 0) {
        buf = func_801b6b64_slot04_0e((Slot04bObj *)o);
        o->variant_index = buf.first;
    }
}

void func_801b2d0c_slot04_0e(Object *obj) {
    data_801c6034_slot04_0e[obj->state_3](obj);
}

void func_801b2d4c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    obj->state_flag = 0;
    o->active_flag_4 = 1;
    o->state_3++;
    func_80141f28(o, 2);
    func_80138ae8(&game_state, o);
    func_801b6d7c_slot04_0e((Slot04bObj *)o);
    if (o->field_49 != 0) {
        a = 0x5e;
    } else {
        a = 0x2c;
    }
    func_801307e0(o, a);
    play_side_sound_at(o, obj->field_a6, 9);
}

void func_801b2dd4_slot04_0e(Object *o) {
    int d;
    int a;

    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        o->active_state = 1;
        o->accel_y = -0x4800;
        o->state_3++;
        o->vel_y = 0x68000;
        d = *(s32 *)&o->other->pos_x_frac;
        d -= *(s32 *)&o->pos_x_frac;
        if (d > 0) {
            d = -d;
        }
        d += 0x1800000;
        if (o->facing_side != 0) {
            d = -d;
        }
        a = 0x5f;
        d >>= 3;
        if (o->field_49 == 0) {
            a = 0x2d;
            d >>= 1;
        }
        o->vel_x = d;
        o->accel_x = 0;
        func_801307e0(o, a);
    }
}
