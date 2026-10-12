/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801495f8(Object *object, int x, int y);
void set_sequence_by_side(Object *object, int index);
void func_801b4b8c_slot04_11(Object *obj);
void func_801b511c_slot04_11(Object *obj);
void func_801b5174_slot04_11(Object *obj);
void func_801b51a8_slot04_11(Object *obj);
void func_801b5874_slot04_11(Object *obj);
int func_801b57a8_slot04_11(Object *obj);

extern ObjectFn data_801c77c0_slot04_11[];
extern ObjectFn data_801c77c8_slot04_11[];
extern s32 data_801c77d4_slot04_11[];
extern ObjectFn data_801c77f4_slot04_11[];

void func_801b4ac8_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b4b0c_slot04_11(Object *obj) {
    if (obj->kind == 0x13) {
        func_801b4b8c_slot04_11(obj);
    } else if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b4b8c_slot04_11(Object *obj) {
    data_801c77c0_slot04_11[obj->state_3](obj);
}

void func_801b4bcc_slot04_11(Object *obj) {
    obj->vel_x = 0x50000;
    obj->accel_x = -0x3000;
    obj->vel_y = 0;
    obj->accel_y = 0;
    obj->state_3++;
}

void func_801b4bf4_slot04_11(Object *obj) {
    int a;
    u16 b;

    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
        if ((obj->step_flags & 0x80) == 0) {
            func_801b57a8_slot04_11(obj);
        }
        a = obj->step_flags & 0x7f;
        if (a != 0) {
            b = a;
            obj->step_flags = obj->step_flags & 0xff80;
            if (obj->facing_side == 0) {
                b = -a;
            }
            obj->pos_x = b + obj->pos_x;
        }
    }
}

void func_801b4cb4_slot04_11(Object *obj) {
    data_801c77c8_slot04_11[obj->state_3](obj);
}

void func_801b4cf4_slot04_11(Object *obj) {
    int k;

    obj->state_3++;
    k = obj->kind != 0x11;
    obj->vel_x = data_801c77d4_slot04_11[(k << 2) + 0];
    obj->accel_x = data_801c77d4_slot04_11[(k << 2) + 1];
    obj->vel_y = data_801c77d4_slot04_11[(k << 2) + 2];
    obj->accel_y = data_801c77d4_slot04_11[(k << 2) + 3];
    obj->active_state = 1;
    update_sequence(obj);
}

void func_801b4d84_slot04_11(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0 && func_801b57a8_slot04_11(obj) < 0 && obj->pos_y >= (s16)((Slot04bObj *)obj)->ground_y) {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        func_801209c4(obj);
        set_sequence_by_side(obj, 0x11);
    }
    update_sequence(obj);
}

void func_801b4e14_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b4e74_slot04_11(Object *obj) {
    data_801c77f4_slot04_11[obj->state_3](obj);
}

void func_801b4eb4_slot04_11(Object *obj) {
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

void func_801b4f20_slot04_11(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->countdown = 1;
        obj->state_3++;
        func_801b511c_slot04_11(obj);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b51a8_slot04_11(obj);
        play_side_sound_at(obj, obj->side, 7);
    }
    update_sequence(obj);
}

void func_801b4fb4_slot04_11(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        ((Slot04bObj *)obj)->field_47 = 0x14;
        obj->state_3++;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b503c_slot04_11(Object *obj) {
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
        if (((Slot04bObj *)obj)->field_134 != 0) {
            if (((Slot04bObj *)obj)->saved_count == 0) {
                ((Slot04bObj *)obj)->saved_count = obj->hit_count_3;
            }
        } else if (((Slot04bObj *)obj)->field_47 != 0) {
            ((Slot04bObj *)obj)->field_47--;
        }
        func_80142fe8(obj);
    }
}

void func_801b511c_slot04_11(Object *obj) {
    int a;
    int b;

    if (obj->kind == 0x13) {
        func_801b5174_slot04_11(obj);
    } else {
        a = -3;
        b = 0x4f;
        if (obj->active_state != 0) {
            a = -0xd;
            b = 0x44;
        }
        func_801495f8(obj, a, b);
    }
}

void func_801b5174_slot04_11(Object *obj) {
    if (obj->active_state != 0) {
        func_801495f8(obj, -0xe, 0x47);
    } else {
        func_801495f8(obj, 8, 0x47);
    }
}
