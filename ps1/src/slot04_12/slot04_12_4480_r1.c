/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4e64_slot04_12[];
extern s16 data_801c4e70_slot04_12[];

void func_80131468(Object *object);
void func_80131638(Object *object);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801b45c4_slot04_12(Object *obj);
void func_801b45f4_slot04_12(Object *obj);
void func_801b465c_slot04_12(Object *obj);
void func_801b4584_slot04_12(Object *obj);
void func_801b49b4_slot04_12(Object *obj);
void func_801b4ac0_slot04_12(Object *obj);
void func_801b4ba8_slot04_12(Object *obj);
int func_801b4bcc_slot04_12(Object *obj);

void func_801b4480_slot04_12(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 4) {
        play_side_sound_at(obj, obj->side, 0xb);
    }
    func_801b4ba8_slot04_12(obj);
}

void func_801b44dc_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4544_slot04_12(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b465c_slot04_12(obj);
    } else {
        func_801b4584_slot04_12(obj);
    }
}

void func_801b4584_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b45c4_slot04_12(obj);
    } else {
        func_801b45f4_slot04_12(obj);
    }
}

void func_801b45c4_slot04_12(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    func_801b4ba8_slot04_12(obj);
}

void func_801b45f4_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b465c_slot04_12(Object *obj) {
    data_801c4e64_slot04_12[obj->state_3](obj);
}

void func_801b469c_slot04_12(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 4 && obj->attack_param_d != 0) {
        obj->active_flag_3 = 1;
        obj->state_3++;
        func_80141f28(obj, 2);
        obj->vel_y = 0x78000;
        obj->accel_x = 0;
        obj->accel_y = -0x5000;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x18000;
        } else {
            obj->vel_x = 0xfffe8000;
        }
        obj->active_state = 1;
        obj->field_157 = 0;
        obj->pos_y -= 0x10;
        func_801307e0(obj, 0x1b);
    } else {
        func_801b4ba8_slot04_12(obj);
    }
}

void func_801b4770_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b47d8_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (func_801b4bcc_slot04_12(obj) < 0 && obj->ground_y <= obj->pos_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->facing_side = obj->facing_side ^ 1;
        func_801209c4(obj);
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b485c_slot04_12(Object *obj) {
    int t = 0xc;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    if (obj->input_dir_second == 0) {
        if (obj->direction_index != 0) {
            if (obj->hit_flag_a != 0 && obj->ground_y - 0x30 < obj->pos_y) {
                if (func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                    obj->state_0 = 1;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    return;
                }
            }
        }
    } else if (obj->direction_index == 2 && obj->attack_param_d != 0) {
        func_801b49b4_slot04_12(obj);
        return;
    }
    func_80141f28(obj, data_801c4e70_slot04_12[obj->direction_index >> 1]);
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    func_801307e0(obj, (u16)((obj->direction_index >> 1) + t));
}

void func_801b49b4_slot04_12(Object *obj) {
    int t = 0x1c;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    func_80141f28(obj, 1);
    obj->step_limit = 0;
    obj->active_flag = 0;
    if (obj->variant_index != 0) {
        t = 0x1d;
    }
    func_801307e0(obj, t);
}

void func_801b4a24_slot04_12(Object *obj) {
    if (obj->step_limit == 0) {
        if (obj->active_flag != 0) {
            func_801b4ac0_slot04_12(obj);
        } else if (func_801b4bcc_slot04_12(obj) < 0 && obj->pos_y >= obj->ground_y) {
            func_801209c4(obj);
            func_80131638(obj);
        } else {
            update_sequence(obj);
        }
    }
}
