/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c62b8_slot04_0f[];
extern ObjectFn data_801c62c4_slot04_0f[];
extern ObjectFn data_801c62d0_slot04_0f[];
extern ObjectFn data_801c62d8_slot04_0f[];
extern ObjectFn data_801c62e4_slot04_0f[];
extern ObjectFn data_801c62ec_slot04_0f[];
extern ObjectFn data_801c62f8_slot04_0f[];
extern ObjectFn data_801c6300_slot04_0f[];
extern ObjectFn data_801c630c_slot04_0f[];
extern s32 data_801c6324_slot04_0f[];

void func_80130dc0(Object *obj);
void func_80131468(Object *object);
void func_801b0e68_slot04_0f(Object *obj);
void func_801b1cc4_slot04_0f(Object *obj);
void func_801b67b0_slot04_0f(Object *obj);
void func_801b5e40_slot04_0f(Object *obj);
void func_801b5e84_slot04_0f(Object *obj);
void func_801b601c_slot04_0f(Object *obj);
void func_801b6060_slot04_0f(Object *obj);
void func_801b61f4_slot04_0f(Object *obj);
void func_801b6238_slot04_0f(Object *obj);
void func_801b638c_slot04_0f(Object *obj);
void func_801b63d0_slot04_0f(Object *obj);
void func_801b66ac_slot04_0f(Object *obj);

void func_801b5dbc_slot04_0f(Object *obj) {
    data_801c62b8_slot04_0f[obj->input_dir_0 >> 1](obj);
}

void func_801b5e00_slot04_0f(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b601c_slot04_0f(obj);
    } else {
        func_801b5e40_slot04_0f(obj);
    }
}

void func_801b5e40_slot04_0f(Object *obj) {
    data_801c62c4_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b5e84_slot04_0f(Object *obj) {
    data_801c62d0_slot04_0f[obj->state_3](obj);
}

void func_801b5ec4_slot04_0f(Object *obj) {
    func_801b5e84_slot04_0f(obj);
}

void func_801b5ee4_slot04_0f(Object *obj) {
    func_801b5e84_slot04_0f(obj);
}

void func_801b5f04_slot04_0f(Object *obj) {
    u8 t;

    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->hit_flag_a == 0 || !func_8013f8c4(obj, -0x14, 0x14)) {
        obj->active_flag_3 = 1;
        if (obj->attack_param_d != 0) {
            t = obj->direction_index >> 1;
            func_80130ec0(obj);
            func_801307e0(obj, t + 0x1a);
        } else {
            func_80130dc0(obj);
        }
    } else {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}

void func_801b5fbc_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b601c_slot04_0f(Object *obj) {
    data_801c62d8_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b6060_slot04_0f(Object *obj) {
    data_801c62e4_slot04_0f[obj->state_3](obj);
}

void func_801b60a0_slot04_0f(Object *obj) {
    func_801b6060_slot04_0f(obj);
}

void func_801b60c0_slot04_0f(Object *obj) {
    func_801b6060_slot04_0f(obj);
}

void func_801b60e0_slot04_0f(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->attack_param_d != 0) {
        func_80130ec0(obj);
        func_801307e0(obj, (obj->direction_index >> 1) + 0x1d);
    } else {
        func_80130dc0(obj);
    }
}

void func_801b6154_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b61b4_slot04_0f(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b638c_slot04_0f(obj);
    } else {
        func_801b61f4_slot04_0f(obj);
    }
}

void func_801b61f4_slot04_0f(Object *obj) {
    data_801c62ec_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b6238_slot04_0f(Object *obj) {
    data_801c62f8_slot04_0f[obj->state_3](obj);
}

void func_801b6278_slot04_0f(Object *obj) {
    func_801b6238_slot04_0f(obj);
}

void func_801b6298_slot04_0f(Object *obj) {
    func_801b6238_slot04_0f(obj);
}

void func_801b62b8_slot04_0f(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->attack_param_d == 0) {
        func_80130dc0(obj);
    } else {
        func_80130ec0(obj);
        func_801307e0(obj, (obj->direction_index >> 1) + 0x20);
    }
}

void func_801b632c_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b638c_slot04_0f(Object *obj) {
    data_801c6300_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b63d0_slot04_0f(Object *obj) {
    data_801c630c_slot04_0f[obj->state_3](obj);
}

void func_801b6410_slot04_0f(Object *obj) {
    func_801b63d0_slot04_0f(obj);
}

void func_801b6430_slot04_0f(Object *obj) {
    func_801b63d0_slot04_0f(obj);
}

void func_801b6450_slot04_0f(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->attack_param_d == 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80130dc0(obj);
    } else {
        func_80130ec0(obj);
        func_801307e0(obj, (obj->direction_index >> 1) + 0x23);
    }
}

void func_801b64d0_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b6530_slot04_0f(Object *obj) {
    int a;
    int b;

    obj->vel_y = 0;
    obj->accel_y = 0;
    obj->state_3 = obj->state_3 + 1;
    a = data_801c6324_slot04_0f[obj->direction_index >> 1];
    b = 0xffff0000;
    if (obj->facing_side == 0) {
        a = -a;
        b = 0x10000;
    }
    obj->vel_x = a;
    obj->accel_x = b;
    update_sequence(obj);
}

void func_801b6598_slot04_0f(Object *obj) {
    if (((Slot04bObj *)obj)->mode != 0) {
        obj->state_3 = obj->state_3 + 1;
    } else {
        update_sequence(obj);
    }
}

void func_801b65dc_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (obj->vel_x == 0) {
        obj->state_3 = obj->state_3 + 1;
        func_801307e0(obj, (obj->direction_index >> 1) + 0x36);
    } else {
        update_sequence(obj);
    }
}

void func_801b664c_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_80131468(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b668c_slot04_0f(Object *obj) {
    func_801b0e68_slot04_0f(obj);
}

void func_801b66ac_slot04_0f(Object *obj) {
    if (obj->active_state != 0 && obj->hit_flag_a != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        obj->variant_index = 0;
        obj->facing_side = obj->saved_facing;
        if (obj->input_dir_second != 0) {
            obj->variant_index = 2;
        }
        if (obj->direction_flag != 0) {
            obj->variant_index = obj->variant_index + 4;
        }
        func_801b1cc4_slot04_0f(obj);
    } else {
        obj->script_opcode_high = obj->script_opcode_high & 0xfe;
        obj->limit_lo = obj->limit_lo | 0x40;
        func_801b67b0_slot04_0f(obj);
    }
}

void func_801b676c_slot04_0f(Object *obj) {
    if (obj->limit_lo & 1) {
        func_801b66ac_slot04_0f(obj);
    } else {
        func_801b67b0_slot04_0f(obj);
    }
}
