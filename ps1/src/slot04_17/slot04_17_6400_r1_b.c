/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ce9f0_slot04_17[];
extern ObjectFn data_801cea00_slot04_17[];
extern ObjectFn data_801cea08_slot04_17[];
extern s16 data_801cea10_slot04_17[];
extern ObjectFn data_801cea18_slot04_17[];
extern ObjectFn data_801cea24_slot04_17[];
extern ObjectRef data_80190458;
extern ObjectRef data_80190414;

void func_8011f38c(Object *o);
void func_80130dc0(Object *obj);
void func_80131468(Object *obj);
void func_801b63fc(Object *obj);

void func_801b5a34_slot04_17(Object *obj);
void func_801b7684_slot04_17(Object *obj);
void func_801b76a8_slot04_17(Object *obj);

void func_801b6c24_slot04_17(Object *obj);
void func_801b6cbc_slot04_17(Object *obj);
void func_801b6d14_slot04_17(Object *obj);
void func_801b6d98_slot04_17(Object *obj);
void func_801b6ec8_slot04_17(Object *obj);
void func_801b7088_slot04_17(Object *obj);
void func_801b70ac_slot04_17(Object *obj);
void func_801b70cc_slot04_17(Object *obj);
void func_801b70ec_slot04_17(Object *obj);

void func_801b6b24_slot04_17(Object *obj) {
    ref_other.p = obj->parent;
    data_80190458.p = ref_other.p->parent;
    data_80190414.p = obj->other;
    data_801ce9f0_slot04_17[obj->state_0](obj);
}

void func_801b6ba0_slot04_17(Object *obj) {
    obj->field_01 = 1;
    obj->image_row = 2;
    obj->field_0c = 0;
    obj->field_20 = 0;
    obj->table_index_2 = 0;
    obj->ot_depth = 0;
    obj->state_0++;
    obj->field_1c = ref_other.p->field_1c;
    func_801b6cbc_slot04_17(obj);
    func_80130768(obj, 1, seqs_8017c7f8);
    func_801b6c24_slot04_17(obj);
}

void func_801b6c24_slot04_17(Object *obj) {
    func_801b6cbc_slot04_17(obj);
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    advance_sequence_checked(obj);
}

void func_801b6c74_slot04_17(Object *o) {
    Object **p = (Object **)ref_other.p->parent;
    int i;

    i = 4;
    do {
        if (p == (Object **)o) {
            *(u32 *)o = 0;
            break;
        }
        p++;
    } while (--i != 0);
    func_8011f38c(o);
}

void func_801b6cbc_slot04_17(Object *obj) {
    obj->pos_x = (u16)ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y + 0x10;
    if (obj->table_index != 0) {
        obj->pos_x = obj->pos_x - 0xc;
        obj->pos_y = obj->pos_y - 6;
    }
}

void func_801b6d14_slot04_17(Object *obj) {
    if (obj->attack_param_d != 0) {
        obj->kind = 0x13;
        func_801b5a34_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b70ec_slot04_17(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b6ec8_slot04_17(obj);
    } else {
        func_801b6d98_slot04_17(obj);
    }
}

void func_801b6d98_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801cea00_slot04_17[obj->state_3](obj);
}

void func_801b6dd8_slot04_17(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 0 || obj->hit_flag_a == 0 || (u8)func_8013f8c4(obj, -0x17, 0x14) == 0) {
        func_801b7088_slot04_17(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}

void func_801b6e64_slot04_17(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b70ac_slot04_17(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b6ec8_slot04_17(Object *obj) {
    obj->field_157 = 1;
    data_801cea08_slot04_17[obj->state_3](obj);
}

void func_801b6f0c_slot04_17(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    func_801b7088_slot04_17(obj);
}

void func_801b6f3c_slot04_17(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b70cc_slot04_17(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b6fa0_slot04_17(Object *object) {
    int t;

    if (object->attack_param_d != 0) {
        object->kind = 0x13;
        func_801b5a34_slot04_17(object);
        select_box_tables(object);
        build_metrics(object);
        func_801b63fc(object);
        return;
    }
    if (object->limit_lo & 1) {
        object->input_dir_second = 2;
    }
    object->state_3 = 3;
    object->input_dir_0 = 4;
    object->active_flag_3 = 1;
    func_80141f28(object, *(s16 *) ((char *) data_801cea10_slot04_17 + (object->direction_index & 0xfe)));
    t = 0xc;
    if (object->variant_index != 0) {
        t = 0x12;
    }
    if (object->input_dir_second != 0) {
        t += 3;
    }
    func_801307e0(object, (object->direction_index >> 1) + t);
}

void func_801b7088_slot04_17(Object *object) {
    object->active_flag_3 = 1;
    func_80130dc0(object);
}

void func_801b70ac_slot04_17(Object *obj) {
    func_801312b8(obj);
}

void func_801b70cc_slot04_17(Object *obj) {
    func_80131468(obj);
}

void func_801b70ec_slot04_17(Object *obj) {
    if (obj->attack_param_d == 0) {
        obj->kind = 0x11;
        func_801b5a34_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b6d14_slot04_17(obj);
    } else {
        data_801cea18_slot04_17[obj->input_dir_0 >> 1](obj);
    }
}

void func_801b7174_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801cea24_slot04_17[obj->state_3](obj);
}

void func_801b71b4_slot04_17(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x17, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b7684_slot04_17(obj);
        func_801b76a8_slot04_17(obj);
    }
}
