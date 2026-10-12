/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c63b0_slot04_0f[];
extern ObjectFn data_801c63c0_slot04_0f[];
extern ObjectFn data_801c63cc_slot04_0f[];
extern SequenceStep **data_1f8000b0;
extern SequenceStep **data_1f800160;

void func_8011ffdc(Object *o);
void func_8011f240(Slab172 *s);
void func_801b6818_slot04_0f(Object *obj);
void func_801b68d8_slot04_0f(Object *obj);
void func_801b6cf4_slot04_0f(Object *obj);
void func_801b6e78_slot04_0f(Object *obj);

void func_801b67b0_slot04_0f(Object *obj) {
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    obj->input_dir_0 = 4;
    func_80120af8(obj);
    if (obj->input_dir_second != 0) {
        func_801b68d8_slot04_0f(obj);
    } else {
        func_801b6818_slot04_0f(obj);
    }
}

void func_801b6818_slot04_0f(Object *obj) {
    int t;

    if (obj->direction_index != 0) {
        func_80141f28(obj, 1);
    }
    t = 0xc;
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->attack_param_d == 0) {
        if (obj->hit_flag_a != 0 && obj->pos_y < obj->ground_y - 0x20) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 5;
            obj->state_3 = 0;
            t = 0x32;
            goto call;
        }
    } else {
        t += 0x1a;
    }
    t += obj->direction_index >> 1;
call:
    func_801307e0(obj, t);
}

void func_801b68d8_slot04_0f(Object *obj) {
    int t;

    if (obj->direction_index != 0) {
        func_80141f28(obj, 1);
    }
    t = 0xf;
    if (obj->variant_index != 0) {
        t = 0x15;
    }
    if (obj->attack_param_d != 0) {
        t += 0x1a;
    } else if (obj->hit_flag_a != 0 && obj->pos_y < obj->ground_y - 0x20) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 5;
        obj->state_3 = 0;
        t = 0x33;
    }
    func_801307e0(obj, t + (obj->direction_index >> 1));
}

void func_801b6994_slot04_0f(Object *obj) {
    ref_other.p = obj->parent;
    data_801c63b0_slot04_0f[obj->state_0](obj);
}

void func_801b69e4_slot04_0f(Object *obj) {
    SequenceStep **t;

    obj->state_0++;
    if (ref_other.p->side == 0) {
        t = data_1f8000b0;
    } else {
        t = data_1f800160;
    }
    func_80130768(obj, 0, t);
}

void func_801b6a48_slot04_0f(Object *obj) {
    data_801c63c0_slot04_0f[obj->state_1](obj);
}

void func_801b6a88_slot04_0f(Object *obj) {
    ref_other.p = obj->parent;
    if ((u8)ref_other.p->step_flags != 0) {
        obj->state_1 = obj->state_1 + 1;
        func_8011ffdc(obj);
    }
}

void func_801b6ae0_slot04_0f(Object *obj) {
    int a;

    advance_sequence_checked(obj);
    if ((s16)obj->step_flags < 0) {
        obj->countdown = 0x1a;
        obj->state_1 = obj->state_1 + 1;
        a = 0x8000;
        if (obj->facing_side != 0) {
            a = -0x8000;
        }
        obj->vel_x = a;
        obj->vel_y = 0;
        obj->accel_y = 0x1000;
    }
    func_8011ffdc(obj);
}

void func_801b6b50_slot04_0f(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    obj->countdown = (s16)obj->countdown - 1;
    t = obj->countdown;
    if (t == 0) {
        obj->state_0 = obj->state_0 + 1;
    } else {
        if (t < 0x12 && (t & 1) == 0) {
            obj->field_01 = 0;
        } else {
            obj->field_01 = 1;
            func_8011ffdc(obj);
        }
    }
}

void func_801b6bf4_slot04_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b6c14_slot04_0f(Object *obj) {
}

void func_801b6c1c_slot04_0f(Object *obj) {
    ref_other.p = obj->parent;
    data_801c63cc_slot04_0f[obj->state_0](obj);
}

void func_801b6c6c_slot04_0f(Object *obj) {
    obj->countdown = 0x20;
    obj->spawn_flag = 1;
    obj->state_0 = obj->state_0 + 1;
    if (ref_other.p->side == 0) {
        func_80130768(obj, 0xc, seqs_a4_left);
    } else {
        func_80130768(obj, 0xc, seqs_154_right);
    }
    func_801b6cf4_slot04_0f(obj);
}

void func_801b6cf4_slot04_0f(Object *obj) {
    if (game_state.config->pause_flag_a == 0 && ref_other.p->field_6b == 0) {
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown == 0) {
            obj->state_0 = obj->state_0 + 1;
            if (ref_other.p->side == 0) {
                func_80130768(obj, 0x11, seqs_a4_left);
            } else {
                func_80130768(obj, 0x11, seqs_154_right);
            }
        }
        func_801b6e78_slot04_0f(obj);
        advance_sequence_checked(obj);
    }
    func_8011ffdc(obj);
}

void func_801b6dcc_slot04_0f(Object *obj) {
    if (game_state.config->pause_flag_a == 0 && ref_other.p->field_6b == 0) {
        advance_sequence_checked(obj);
        func_801b6e78_slot04_0f(obj);
        if ((s16)obj->step_flags < 0) {
            obj->state_0 = obj->state_0 + 1;
        }
    }
    func_8011ffdc(obj);
}

void func_801b6e58_slot04_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b6e78_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
}
