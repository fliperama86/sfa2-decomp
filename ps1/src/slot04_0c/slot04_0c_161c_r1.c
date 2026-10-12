/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bd9d0_slot04_0c[];
extern ObjectFn data_801bda10_slot04_0c[];
extern ObjectFn data_801bda20_slot04_0c[];
extern s32 data_801bd9e0_slot04_0c[];
void func_80142adc(Object *object);
void func_80146998(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801483a4(Object *object, int a, int b);

void func_801b161c_slot04_0c(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->countdown = 5;
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xc;
            p->table_index = 0;
            p->team_id = obj->team_id;
            p->sound_side = obj->sound_side;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->health = 0;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x15);
        }
    }
}

void func_801b1754_slot04_0c(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t != 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        func_80142adc(obj);
    tail:
        update_sequence(obj);
    }
}

void func_801b17d4_slot04_0c(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    }
}

void func_801b183c_slot04_0c(Object *obj) {
    data_801bd9d0_slot04_0c[obj->state_3](obj);
}

void func_801b187c_slot04_0c(Object *obj) {
    int n = 0x22;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        n = 0x39;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + n);
}

void func_801b18fc_slot04_0c(Object *obj) {
    int i;
    s32 a;
    s32 b;

    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->state_3++;
        obj->pos_y_frac = 0;
        i = obj->direction_index;
        a = data_801bd9e0_slot04_0c[i * 2];
        b = data_801bd9e0_slot04_0c[i * 2 + 1];
        obj->vel_y = data_801bd9e0_slot04_0c[i * 2 + 2];
        obj->accel_y = data_801bd9e0_slot04_0c[i * 2 + 3];
        if (obj->facing_side == 0) {
            obj->vel_x = -a;
            obj->accel_x = -b;
        } else {
            obj->vel_x = a;
            obj->accel_x = b;
        }
    }
    update_sequence(obj);
}

void func_801b19b8_slot04_0c(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = obj->ground_y;
        func_801209c4(obj);
        obj->field_157 = 1;
        func_801307e0(obj, 0x28);
    }
}

void func_801b1a74_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b1acc_slot04_0c(Object *obj) {
    data_801bda10_slot04_0c[obj->state_3](obj);
}

void func_801b1b0c_slot04_0c(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x29);
}

void func_801b1b60_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_801483a4(obj, 0x1e, 0x57);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}

void func_801b1be4_slot04_0c(Object *obj) {
    int s1 = 0;

    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            s1 = 4;
            obj->other->field_6b = 0xa;
        }
        obj->other_lock = s1;
    }
}

void func_801b1c54_slot04_0c(Object *obj) {
    Object *p;
    u16 t;
    s16 m;
    s16 y;

    update_sequence(obj);
    t = obj->step_flags;
    m = t & 0xff00;
    if (m < 0) {
        func_801312b8(obj);
    } else if (m == 0 && (t & 0xff) != 0) {
        obj->step_flags = 0;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xc;
            p->table_index = 2;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->sound_side = obj->sound_side;
            p->vel_table_index = obj->direction_index + 6;
            p->health = (obj->direction_index >> 1) + 2;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x15);
        }
    }
}

void func_801b1dd0_slot04_0c(Object *obj) {
    data_801bda20_slot04_0c[obj->state_3](obj);
}
