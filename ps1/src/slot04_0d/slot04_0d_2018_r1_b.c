/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2a60_slot04_0d[];
extern ObjectFn data_801c2a74_slot04_0d[];
extern u8 data_801c2804_slot04_0d[];
void func_80142adc(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);

void func_801b22b8_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        func_801312b8(obj);
    }
}

void func_801b2314_slot04_0d(Object *obj) {
    data_801c2a60_slot04_0d[obj->state_3](obj);
}

void func_801b2354_slot04_0d(Object *obj) {
    obj->state_3++;
    *(u8 *)&obj->countdown = 0x32;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x3e);
}

void func_801b23b4_slot04_0d(Object *obj) {
    int a;

    (*(u8 *)&obj->countdown)--;
    if (*(u8 *)&obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, 0x11, 0x40);
    }
    update_sequence(obj);
}

void func_801b2440_slot04_0d(Object *obj) {
    u8 a = 0;

    *(u8 *)&obj->countdown -= 1;
    if (*(u8 *)&obj->countdown & 0x80) {
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c2804_slot04_0d[a];
        func_801307e0(obj, 0x35);
    } else {
        update_sequence(obj);
    }
}

void func_801b24d8_slot04_0d(Object *obj) {
    Object *p;
    s8 t;
    s32 w;
    int one;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            one = 1;
            p->active_count = one;
            p->field_02 = 0xd;
            p->table_index = 2;
            p->sound_side = obj->sound_side;
            p->field_4b = obj->field_4b;
            p->team_id = obj->team_id;
            t = obj->direction_index;
            p->active_flag_2 = one;
            p->vel_table_index = t + 6;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            w = obj->data_ptr_b;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->data_ptr_b = w;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->health = (t >> 1) + 2;
            p->parent = obj;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x14);
        }
    }
}

void func_801b2620_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2660_slot04_0d(Object *obj) {
    data_801c2a74_slot04_0d[obj->state_3](obj);
}

void func_801b26a0_slot04_0d(Object *obj) {
    obj->other_state_code = 2;
    *(s32 *)&obj->vel_x = 0x80000;
    obj->accel_x = -0x8000;
    obj->state_3++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x38);
}

void func_801b2718_slot04_0d(Object *obj) {
    int a;

    obj->other_state_code = 2;
    if (*(u8 *)&obj->step_flags != 0) {
        a = 1;
        obj->state_3++;
        if (obj->field_4b == 0) {
            a = -1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, 0xf, 0x44);
    }
    update_sequence(obj);
}
