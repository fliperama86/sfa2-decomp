/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6e08_slot04_10[];
extern u8 data_801ba218_slot04_10[];
extern u32 data_801c68bc_slot04_10[];
extern ObjectFn data_801c695c_slot04_10[];
extern ObjectFn data_801c6964_slot04_10[];
extern ObjectFn data_801c696c_slot04_10[];
extern ObjectFn data_801c6974_slot04_10[];
extern ObjectFn data_801c697c_slot04_10[];

Object *func_8011f32c(void);
int func_80141618(Object *object);
int func_801412a4(Object *object);
void func_80130dc0(Object *object);
void func_80131468(Object *object);

void func_801b0414_slot04_10(Object *obj);
void func_801b04ec_slot04_10(Object *obj);
void func_801b0334_slot04_10(Object *obj);
void func_801b0648_slot04_10(Object *obj);
void func_801b02f4_slot04_10(Object *obj);
void func_801b07e4_slot04_10(Object *obj);
void func_801b0688_slot04_10(Object *obj);
void func_801b090c_slot04_10(Object *obj);
void func_801b1078_slot04_10(Object *obj);

void func_801b0118_slot04_10(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b6e08_slot04_10;
    obj->data_ptr_b = data_801ba218_slot04_10;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c68bc_slot04_10[i];
    }
    ref_other.p = func_8011f32c();
    if (ref_other.p != 0) {
        ref_other.p->active_count = 1;
        ref_other.p->field_02 = 0x10;
        ref_other.p->parent = obj;
        ref_other.p->field_0c = obj->field_0c;
        ref_other.p->field_0e = obj->field_0e;
        obj->linked_object = (u32)ref_other.p;
        ref_other.p->sound_side = obj->sound_side;
        ref_other.p->image_row = obj->image_row;
        ref_other.p->spawn_x = obj->spawn_x;
        ref_other.p->spawn_y = obj->spawn_y;
        ref_other.p->field_90 = obj->field_90;
        ref_other.p->data_ptr_a = obj->data_ptr_a;
        ref_other.p->data_ptr_b = obj->data_ptr_b;
    }
}

void func_801b029c_slot04_10(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b090c_slot04_10(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b0648_slot04_10(obj);
    } else {
        func_801b02f4_slot04_10(obj);
    }
}

void func_801b02f4_slot04_10(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b04ec_slot04_10(obj);
    } else {
        func_801b0334_slot04_10(obj);
    }
}

void func_801b0334_slot04_10(Object *obj) {
    data_801c695c_slot04_10[obj->state_3](obj);
}

void func_801b0374_slot04_10(Object *obj) {
    int one;

    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x25, 0x14) != 0) {
        func_801b0414_slot04_10(obj);
    } else {
        one = 1;
        obj->active_flag_3 = one;
        func_80130dc0(obj);
        if (obj->direction_index == 4) {
            obj->field_278 = one;
        }
    }
}

void func_801b0414_slot04_10(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 2;
    obj->state_2 = 0;
    obj->state_3 = 0;
}

void func_801b0430_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        func_801b1078_slot04_10(obj);
    } else {
        if ((t & 1) != 0) {
            s16 d = 3;
            if (obj->facing_side == 0) {
                d = -3;
            }
            obj->pos_x = d + obj->pos_x;
        }
        if ((obj->step_flags & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b04ec_slot04_10(Object *obj) {
    data_801c6964_slot04_10[obj->state_3](obj);
}

void func_801b052c_slot04_10(Object *obj) {
    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) == 0) {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
        return;
    }
    if (obj->direction_index != 0 && (u8)func_8013f8c4(obj, -0x25, 0x14) != 0) {
        func_801b0414_slot04_10(obj);
        return;
    }
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}

void func_801b05c4_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        func_801b1078_slot04_10(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b0648_slot04_10(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b07e4_slot04_10(obj);
    } else {
        func_801b0688_slot04_10(obj);
    }
}

void func_801b0688_slot04_10(Object *obj) {
    data_801c696c_slot04_10[obj->state_3](obj);
}

void func_801b06c8_slot04_10(Object *obj) {
    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index == 0) {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
        return;
    }
    if ((obj->input_bits_2 & 0x8000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x25, 0x14) != 0) {
            func_801b0414_slot04_10(obj);
            return;
        }
    }
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}

void func_801b0760_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b07e4_slot04_10(Object *obj) {
    data_801c6974_slot04_10[obj->state_3](obj);
}

void func_801b0824_slot04_10(Object *obj) {
    unsigned a;

    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    a = obj->direction_index;
    obj->active_flag = 0;
    if (a == 0 || (obj->input_bits_2 & 0x2000) == 0) {
        func_80130dc0(obj);
    } else {
        func_801307e0(obj, (a >> 1) + 0x55);
    }
}

void func_801b0888_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b090c_slot04_10(Object *obj) {
    data_801c697c_slot04_10[obj->step_limit](obj);
}
