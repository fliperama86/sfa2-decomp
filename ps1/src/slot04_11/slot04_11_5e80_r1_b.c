/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7a34_slot04_11[];
extern ObjectFn data_801c7a40_slot04_11[];
extern ObjectFn data_801c7a48_slot04_11[];
extern u8 data_801c7a50_slot04_11[];

void func_80131468(Object *obj);
void func_80131638(Object *obj);
void func_8007bc00(Object *obj);
void func_8007be8c(Object *obj);
void func_801b5ab8_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);
void func_801b655c_slot04_11(Object *obj);

void func_801b600c_slot04_11(Object *obj) {
    func_801312b8(obj);
}

void func_801b602c_slot04_11(Object *obj) {
    func_80131468(obj);
}

void func_801b604c_slot04_11(Object *obj) {
    func_80131638(obj);
}

void func_801b606c_slot04_11(Object *obj) {
    if (obj->attack_param_d == 0) {
        obj->kind = 0x11;
        func_801b5ab8_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_8007bc00(obj);
    } else {
        data_801c7a34_slot04_11[obj->input_dir_0 >> 1](obj);
    }
}

void func_801b60f4_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7a40_slot04_11[obj->state_3](obj);
}

void func_801b6134_slot04_11(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x17, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b6604_slot04_11(obj);
        func_801b6628_slot04_11(obj);
    }
}

void func_801b61c4_slot04_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b66ac_slot04_11(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
        func_801b6628_slot04_11(obj);
    }
}

void func_801b6230_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c7a48_slot04_11[obj->state_3](obj);
}

void func_801b6274_slot04_11(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->input_dir_second != 0 && obj->direction_index == 4) {
        obj->field_29c = 1;
    }
    func_801b6604_slot04_11(obj);
    func_801b6628_slot04_11(obj);
}

void func_801b62d8_slot04_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b66cc_slot04_11(obj);
    }
    if (func_80149b80(obj) & 0xff) {
        obj->state_3 = 0;
    }
    update_sequence(obj);
    func_801b6628_slot04_11(obj);
}

void func_801b633c_slot04_11(Object *obj) {
    int n;

    if (obj->active_flag != 0 && *(u8 *)&obj->step_flags != 0 && obj->hit_flag_a != 0) {
        n = 0x18;
        if (obj->variant_index != 0) {
            n = 0x19;
        }
        func_801307e0(obj, n);
    } else if (func_801b6668_slot04_11(obj) < 0 && obj->pos_y >= obj->ground_y) {
        func_801209c4(obj);
        func_801b66ec_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b63fc_slot04_11(Object *obj) {
    if (obj->attack_param_d == 0) {
        obj->kind = 0x11;
        func_801b5ab8_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_8007be8c(obj);
    } else if (obj->limit_lo != 0) {
        func_801b655c_slot04_11(obj);
    } else {
        func_801b6480_slot04_11(obj);
    }
}

void func_801b6480_slot04_11(Object *obj) {
    s16 n;
    s16 t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    func_80141f28(obj, *(s16 *)((u8 *)data_801c7a50_slot04_11 + (obj->direction_index & 0xfe)));
    n = 0xc;
    if (obj->variant_index != 0) {
        n = 0x12;
    }
    if (obj->input_dir_second != 0) {
        n += 3;
    }
    t = obj->direction_index >> 1;
    t += n;
    func_801307e0(obj, t);
    if (obj->input_dir_second != 0 && obj->direction_index == 4) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 5;
        obj->state_3 = 0;
        obj->active_flag = 0;
    }
}
