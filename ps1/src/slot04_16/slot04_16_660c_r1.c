/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ca604_slot04_16[];
extern ObjectFn data_801ca630_slot04_16[];

void func_801b090c_slot04_16(Object *obj);
void func_801b0c38_slot04_16(Object *obj);
void func_801b0c8c_slot04_16(Object *obj);
void func_801b0ce8_slot04_16(Object *obj);
void func_8011f38c(Object *o);
void func_8011f14c(Slab172 *s);
void func_80130dc0(Object *obj);
void func_80131468(Object *obj);
void func_801b6830_slot04_16(Object *obj);
void func_801b6888_slot04_16(Object *obj);
void func_801b6890_slot04_16(Object *obj, s16 idx);
void func_801b6918_slot04_16(Object *obj);
void func_801b6958_slot04_16(Object *obj);
void func_801b69a0_slot04_16(Object *obj);
void func_801b6a28_slot04_16(Object *obj);
void func_801b6a44_slot04_16(Object *obj);
void func_801b6ad8_slot04_16(Object *obj);
void func_801b6b20_slot04_16(Object *obj);
void func_801b6ba8_slot04_16(Object *obj);
void func_801b6c0c_slot04_16(Object *obj);
void func_801b6c58_slot04_16(Object *obj);
void func_801b6cf0_slot04_16(Object *obj);

void func_801b660c_slot04_16(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void func_801b6620_slot04_16(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801b6640_slot04_16(Object *obj) {
    ref_other.p = obj->parent;
    data_801ca630_slot04_16[obj->state_0](obj);
}

void func_801b6690_slot04_16(Object *obj) {
    obj->data_ptr_a = ref_other.p->data_ptr_a;
    obj->data_ptr_b = ref_other.p->data_ptr_b;
    obj->state_0 = obj->state_0 + 1;
    obj->field_1c = ref_other.p->field_1c;
    obj->table_index = ref_other.p->kind;
    obj->field_0c = ref_other.p->field_0c;
    obj->field_0e = ref_other.p->field_0e;
    obj->sound_side = ref_other.p->sound_side;
    obj->variant_index = 0;
}

void func_801b6740_slot04_16(Object *obj) {
    obj->field_01 = 0;
    if (ref_other.p->kind != obj->table_index) {
        func_801b6830_slot04_16(obj);
    } else if (ref_other.p->frame->frame_param == 0) {
        func_801b6888_slot04_16(obj);
    } else {
        obj->pos_x = ref_other.p->pos_x;
        obj->pos_y = ref_other.p->pos_y;
        obj->facing_side = ref_other.p->facing_side;
        obj->field_01 = 1;
        if (ref_other.p->frame->frame_param != obj->variant_index) {
            func_801b6890_slot04_16(obj, ref_other.p->frame->frame_param);
        } else {
            advance_sequence_checked(obj);
        }
    }
}

void func_801b6830_slot04_16(Object *obj) {
    obj->state_0++;
}

void func_801b6844_slot04_16(Object *o) {
    Object *p = ref_other.p;

    if (p->linked_object != (s32)o) {
        func_8011f38c(o);
    } else {
        p->linked_object = 0;
    }
}

void func_801b6888_slot04_16(Object *obj) {
    obj->variant_index = 0;
}

void func_801b6890_slot04_16(Object *obj, s16 idx) {
    obj->variant_index = idx;
    func_80130768(obj, idx, data_801ca604_slot04_16);
}

void func_801b68c0_slot04_16(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b090c_slot04_16(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b6c0c_slot04_16(obj);
    } else {
        func_801b6918_slot04_16(obj);
    }
}

void func_801b6918_slot04_16(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b6ad8_slot04_16(obj);
    } else {
        func_801b6958_slot04_16(obj);
    }
}

void func_801b6958_slot04_16(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b69a0_slot04_16(obj);
    } else if (obj->state_3 == 1) {
        func_801b6a44_slot04_16(obj);
    }
}

void func_801b69a0_slot04_16(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x26, 0x14) != 0) {
        func_801b6a28_slot04_16(obj);
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b6a28_slot04_16(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 2;
    obj->state_2 = 0;
    obj->state_3 = 0;
}

void func_801b6a44_slot04_16(Object *obj) {
    s16 t = obj->step_flags;
    s16 n;

    if (t < 0) {
        func_801312b8(obj);
    } else {
        if (t & 0x100) {
            n = 3;
            if (obj->facing_side == 0) {
                n = -3;
            }
            obj->pos_x = n + obj->pos_x;
        }
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b6ad8_slot04_16(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b6b20_slot04_16(obj);
    } else if (obj->state_3 == 1) {
        func_801b6ba8_slot04_16(obj);
    }
}

void func_801b6b20_slot04_16(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x24, 0x14) != 0) {
        func_801b6a28_slot04_16(obj);
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b6ba8_slot04_16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b6c0c_slot04_16(Object *obj) {
    obj->field_157 = 1;
    if (obj->state_3 == 0) {
        func_801b6c58_slot04_16(obj);
    } else if (obj->state_3 == 1) {
        func_801b6cf0_slot04_16(obj);
    }
}

void func_801b6c58_slot04_16(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->input_dir_second == 0 && obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x26, 0x14) != 0) {
        func_801b6a28_slot04_16(obj);
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b6cf0_slot04_16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b6d54_slot04_16(Object *obj) {
    s16 n;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    func_80141f28(obj, obj->direction_index >> 1);
    if (obj->attack_param_d != 0) {
        if (obj->input_dir_second != 0) {
            func_801b0c8c_slot04_16(obj);
        } else if (obj->variant_index == 0) {
            func_801b0ce8_slot04_16(obj);
        } else {
            func_801b0c38_slot04_16(obj);
        }
    } else {
        n = 0xc;
        if (obj->variant_index != 0) {
            n = 0x12;
        }
        if (obj->input_dir_second != 0) {
            n += 3;
        }
        n += obj->direction_index >> 1;
        func_801307e0(obj, n);
    }
}
