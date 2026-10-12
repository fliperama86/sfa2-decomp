/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4c10_slot04_12[];
extern s32 data_801c4c28_slot04_12[];
extern u8 data_801c4c58_slot04_12[];
extern ObjectFn data_801c4c5c_slot04_12[];
extern u8 data_801c4c74_slot04_12[];
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
int func_801b3a90_slot04_12(Object *object);

void func_801b276c_slot04_12(Object *obj) {
    data_801c4c10_slot04_12[obj->state_3](obj);
}

void func_801b27ac_slot04_12(Object *o) {
    o->state_3++;
    o->countdown = ((Slot04bObj *)o)->countdown + 0x400;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    if (o->facing_side != 0) {
        o->pos_x = o->pos_x + 0x18;
    } else {
        o->pos_x = o->pos_x - 0x18;
    }
    o->vel_x = data_801c4c28_slot04_12[o->direction_index * 2];
    o->accel_x = data_801c4c28_slot04_12[o->direction_index * 2 + 1];
    o->vel_y = data_801c4c28_slot04_12[o->direction_index * 2 + 2];
    o->accel_y = data_801c4c28_slot04_12[o->direction_index * 2 + 3];
    func_801307e0(o, o->direction_index + 0x53);
}

void func_801b28b8_slot04_12(Object *o) {
    o->countdown = (s16)o->countdown - 0x100;
    if (o->countdown & 0xff00) {
        o->state_3++;
        o->countdown = ((Slot04bObj *)o)->countdown + 0x3800;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        play_side_sound_at(o, o->side, 6);
        play_sound_at(o, o->side, 0x31c);
        func_801483a4(o, -0xc, 0x30);
    }
}

void func_801b295c_slot04_12(Object *o) {
    o->countdown = (s16)o->countdown - 0x100;
    if ((o->countdown & 0xff00) == 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            ((Slot04bObj *)o)->table_value = data_801c4c58_slot04_12[3];
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            ((Slot04bObj *)o)->table_value = data_801c4c58_slot04_12[o->direction_index >> 1];
        }
        o->field_165 = 0;
        play_side_sound_at(o, o->side, 7);
    }
}

void func_801b2a08_slot04_12(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}

void func_801b2a48_slot04_12(Object *o) {
    if (func_801b3a90_slot04_12(o) < 0 && ((s16)o->step_flags & 0x8000)) {
        o->accel_y = -0x6000;
        if (o->pos_y > o->ground_y) {
            o->pos_y_frac = 0;
            o->active_state = 0;
            o->active_flag_3 = 0;
            o->state_3++;
            o->pos_y = o->ground_y;
            func_801209c4(o);
            func_801307e0(o, o->direction_index + 0x54);
            return;
        }
    }
    update_sequence(o);
}

void func_801b2aec_slot04_12(Object *o) {
    if (((s16)o->step_flags & 0x8000) == 0) {
        update_sequence(o);
    } else {
        ref_other.p = o->other;
        ref_other.p->other_state_code = 5;
        func_801312b8(o);
    }
}

void func_801b2b4c_slot04_12(Object *obj) {
    data_801c4c5c_slot04_12[obj->state_3](obj);
}

void func_801b2b8c_slot04_12(Object *o) {
    o->state_3++;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    func_801307e0(o, 0x59);
}

void func_801b2be8_slot04_12(Object *o) {
    update_sequence(o);
    if (*(u8 *)&o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        play_side_sound_at(o, o->side, 8);
        play_sound_at(o, o->side, 0x31c);
        func_801483a4(o, -0x1a, 0x33);
    }
}

void func_801b2c74_slot04_12(Object *o) {
    u8 t;

    update_sequence(o);
    if (*(u8 *)&o->step_flags == 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            t = 3;
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            t = o->direction_index >> 1;
        }
        ((Slot04bObj *)o)->table_value = data_801c4c74_slot04_12[t];
        o->field_165 = 0;
        play_side_sound_at(o, o->side, 9);
    }
}

void func_801b2d20_slot04_12(Object *obj) {
    Object *p;
    s16 y;

    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x1d;
            p->active_flag_2 = 1;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->scroll_factor = obj->scroll_factor;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->ground_y = obj->ground_y;
            p->parent = obj;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            play_side_sound_at(obj, obj->side, 0x11);
        }
        func_801307e0(obj, obj->direction_index + 0x5a);
    }
}
