/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5b08_slot04_0f[];
extern ObjectFn data_801c5b18_slot04_0f[];

void func_80138b38(GameState *state, Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80142adc(Object *object);
void func_80142fe8(Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80142c70(Object *object);
void set_sequence_by_side(Object *object, int arg);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b2ec4_slot04_0f(Object *obj);
void func_801b470c_slot04_0f(Object *obj);

void func_801b270c_slot04_0f(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 6);
    func_80138ae8((GameState *)game_state.config, obj);
    a = 0x3e;
    if (obj->field_49 != 0) {
        a = 0x5a;
    }
    func_801307e0(obj, a + (obj->direction_index >> 1));
}

void func_801b2784_slot04_0f(Object *obj) {
    Object *p;
    s32 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x19;
            p->table_index = 0;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->scroll_factor = obj->scroll_factor;
            *(s32 *)&p->pos_x_frac = *(s32 *)&obj->pos_x_frac;
            y = *(s32 *)&obj->pos_y_frac;
            p->spawn_x = 0x60;
            p->health = 0;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            *(s32 *)&p->pos_y_frac = y;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x11);
        }
    }
}

void func_801b28b8_slot04_0f(Object *obj) {
    if (obj->linked_object_3 == 0) {
        obj->active_flag_4 = 0;
        obj->state_3++;
        func_801307e0(obj, 0x44);
    } else {
        update_sequence(obj);
    }
}

void func_801b2904_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b470c_slot04_0f(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b2958_slot04_0f(Object *obj) {
    data_801c5b08_slot04_0f[obj->state_3](obj);
}

void func_801b2998_slot04_0f(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 6);
    func_80138ae8((GameState *)game_state.config, obj);
    a = 0x41;
    if (obj->field_49 != 0) {
        a = 0x5d;
    }
    func_801307e0(obj, a + (obj->direction_index >> 1));
}

void func_801b2a10_slot04_0f(Object *obj) {
    Object *p;
    s32 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x19;
            p->table_index = 2;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index + 6;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->scroll_factor = obj->scroll_factor;
            *(s32 *)&p->pos_x_frac = *(s32 *)&obj->pos_x_frac;
            y = *(s32 *)&obj->pos_y_frac;
            p->spawn_x = 0x60;
            p->health = 0;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            *(s32 *)&p->pos_y_frac = y;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->input_matched_flag = 1;
            obj->linked_object_3 = (s32)p;
            obj->countdown = 3;
            obj->link_flags++;
        }
        play_side_sound_at(obj, obj->side, 0x11);
    }
}

void func_801b2b5c_slot04_0f(Object *obj) {
    if (obj->linked_object_3 == 0) {
        obj->state_3++;
        func_801307e0(obj, 0x45);
    } else {
        update_sequence(obj);
    }
}

void func_801b2ba8_slot04_0f(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags < 0) {
        func_801b470c_slot04_0f(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t == 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        func_80142adc(obj);
    tail:
        update_sequence(obj);
    }
}

void func_801b2c24_slot04_0f(Object *obj) {
    data_801c5b18_slot04_0f[obj->state_3](obj);
}

void func_801b2c64_slot04_0f(Object *obj) {
    int a;
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38((GameState *)game_state.config, obj);
    if (obj->active_state != 0) {
        a = 0x2f;
    } else {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void func_801b2cd0_slot04_0f(Object *obj) {
    int a = 0;
    int b = 0x55;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state == 0) {
            a = 2;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b2ec4_slot04_0f(obj);
    }
    update_sequence(obj);
}

void func_801b2d5c_slot04_0f(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        a = ((Slot04bObj *)obj)->field_c6;
        obj->state_3++;
        ((Slot04bObj *)obj)->field_47 = 0x14;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        a += 0x1e;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b2de4_slot04_0f(Object *obj) {
    obj->hit_count_3++;
    update_sequence(obj);
    ((Slot04bObj *)obj)->countdown--;
    if (((Slot04bObj *)obj)->countdown == 0) {
        if (obj->active_state != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 3;
            obj->state_3 = 1;
        }
        func_80142c70(obj);
    } else {
        if (obj->pressed_bits != 0) {
            if (((Slot04bObj *)obj)->saved_count == 0) {
                ((Slot04bObj *)obj)->saved_count = obj->hit_count_3;
            }
        } else if (((Slot04bObj *)obj)->field_47 != 0) {
            ((Slot04bObj *)obj)->field_47--;
        }
        func_80142fe8(obj);
    }
}
