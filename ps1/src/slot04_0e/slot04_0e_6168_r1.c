/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c61ac_slot04_0e[];
extern ObjectFn data_801c61b8_slot04_0e[];
extern u16 data_801c5d84_slot04_0e[];
extern u16 data_801a2804[];
extern u16 box_margin[];

Block172 *func_8011f1e0(void);
void set_position_from_other_box(Object *object);
void func_80142adc(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b4914_slot04_0e(Object *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);
BytePair func_801b6b64_slot04_0e(Slot04bObj *obj);
void func_801b6374_slot04_0e(Object *obj);
void func_801b650c_slot04_0e(Object *obj);
void func_801b6698_slot04_0e(Object *obj, u8 a, int b, int c);
void func_801b6808_slot04_0e(Object *obj);
void func_801b687c_slot04_0e(Object *obj);
u8 func_801b6af0_slot04_0e(Object *obj);

void func_801b6168_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->state_3++;
        func_801307e0(obj, 0x2a);
    }
}

void func_801b61b8_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->ground_y >= obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = 7;
        obj->active_state = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801307e0(obj, 0x2b);
    }
}

void func_801b6238_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b628c_slot04_0e(Object *obj) {
    BytePair buf;
    u16 t;

    if (obj->mode_flag_2 != 0) {
        if (func_801b6bd4_slot04_0e(obj) != 0) {
            buf = func_801b6b64_slot04_0e((Slot04bObj *)obj);
            obj->variant_index = buf.first;
            ((Slot04bObj *)obj)->state_flag = 1;
        }
    } else {
        t = obj->pressed_bits;
        if (t & 0x95) {
            buf = func_801b6b64_slot04_0e((Slot04bObj *)obj);
            t = obj->pressed_bits;
            obj->variant_index = buf.first;
            if ((t & 0x94) != 0x94) {
                if ((t & 1) == 0) {
                    ((Slot04bObj *)obj)->state_flag = 1;
                }
            }
        }
    }
}

void func_801b6334_slot04_0e(Object *obj) {
    if (obj->input_dir_second != 0) {
        func_801b650c_slot04_0e(obj);
    } else {
        func_801b6374_slot04_0e(obj);
    }
}

void func_801b6374_slot04_0e(Object *obj) {
    data_801c61ac_slot04_0e[obj->state_3](obj);
}

void func_801b63b4_slot04_0e(Object *obj) {
    Object *o;

    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    o = obj->other;
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if ((u16)(box_margin[0] + 0xc0) < obj->pos_x) {
            goto set;
        }
    } else if (obj->input_bits & 0x8000) {
set:
        obj->facing_side = 1;
    }
    func_801307e0(obj, 0x18);
}

void func_801b645c_slot04_0e(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0, 5, 0x10, 0, 1, 0);
    }
    update_sequence(obj);
}

void func_801b64c0_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        func_801312b8(obj);
    }
}

void func_801b650c_slot04_0e(Object *obj) {
    data_801c61b8_slot04_0e[obj->state_3](obj);
}

void func_801b654c_slot04_0e(Object *obj) {
    Object *o;

    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    o = obj->other;
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if ((u16)(box_margin[0] + 0xc0) < obj->pos_x) {
            goto set;
        }
    } else if (obj->input_bits & 0x8000) {
set:
        obj->facing_side = 1;
    }
    func_801307e0(obj, 0x19);
    set_position_from_other_box(obj);
}

void func_801b65fc_slot04_0e(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        set_position_from_other_box(obj);
    } else {
        *(u8 *)&obj->step_flags = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->other->field_261 = 1;
        func_801b6698_slot04_0e(obj, 0x53, 0, 0);
        func_801b6698_slot04_0e(obj, 0x54, -0x70000, -0x20000);
        func_801b6698_slot04_0e(obj, 0x55, -0x110000, -0x30000);
    }
}

void func_801b6698_slot04_0e(Object *obj, u8 a, int b, int c) {
    Object *p = (Object *)func_8011f1e0();

    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x3b;
        p->table_index = 5;
        p->ot_depth = 0;
        p->field_02 = 0x14;
        p->team_id = obj->team_id;
        p->spawn_x = 0x60;
        p->parent = obj;
        p->variant_index = a;
        p->vel_x = b;
        p->vel_y = c;
        p->spawn_y = 0x1e0;
        p->image_row = obj->image_row;
        p->mode_id = 0x20;
        p->field_90 = obj->field_90;
        p->sound_side = obj->sound_side;
        p->data_ptr_a = obj->data_ptr_a;
        p->data_ptr_b = obj->data_ptr_b;
    }
}

void func_801b6778_slot04_0e(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->vel_x = 0xa0000;
        obj->accel_x = -0x2000;
        obj->accel_y = -0x4000;
        obj->vel_y = 0x28000;
        *(u8 *)&obj->step_flags = 0;
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
    }
}

void func_801b6808_slot04_0e(Object *obj) {
    u16 i = 0;

    do {
        data_801a2804[obj->image_row * 16 + i] = data_801c5d84_slot04_0e[i];
        i++;
    } while (i < 0x10);
    func_80137220(0, 6);
}

void func_801b687c_slot04_0e(Object *obj) {
    func_801378d8(obj);
}

void func_801b689c_slot04_0e(Object *obj) {
    Object *o;

    func_801b4914_slot04_0e(obj);
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        *(u8 *)&obj->step_flags = 0;
        o = obj->other;
        if (o->field_15b != 0) {
            func_801b6808_slot04_0e(obj);
            play_side_sound_at(obj, obj->side, 7);
        }
    }
}

void func_801b6918_slot04_0e(Object *obj) {
    Object *o;

    func_801b4914_slot04_0e(obj);
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        *(u8 *)&obj->step_flags = 0;
        o = obj->other;
        o->field_261 = 0;
        o->pos_x_frac = 0;
        if (o->field_15b != 0) {
            func_80140770(obj, 0, 0xc, 0x10, 0, 1, 0);
        } else {
            func_80140770(obj, 0, 5, -1, 0, 1, 0);
        }
    }
}

void func_801b69b4_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->ground_y < obj->pos_y) {
        obj->active_state = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801b687c_slot04_0e(obj);
        obj->step_timer = 1;
        update_sequence(obj);
    }
}

void func_801b6a30_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        func_801312b8(obj);
    }
}

u8 func_801b6a7c_slot04_0e(Object *obj) {
    u8 r;

    if (obj->input_dir_second == 0 || obj->direction_index != 2) {
        r = 0;
    } else if (obj->mode_flag_2 != 0) {
        r = func_801b6af0_slot04_0e(obj);
    } else {
        r = (obj->input_bits_2 >> 14) & 1;
    }
    return r;
}

u8 func_801b6af0_slot04_0e(Object *obj) {
    return obj->attack_param_d != 0;
}

void func_801b6afc_slot04_0e(Object *obj) {
    obj->vel_x = obj->vel_x >> 1;
    func_80141f28(obj, 1);
    func_801307e0(obj, 0x1a);
}

void func_801b6b40_slot04_0e(Object *obj) {
    int i;
    u8 z = 0;
    u8 *p = (u8 *)obj + 0x2b0;

    for (i = 0x4f; i >= 0; i--) {
        *p++ = z;
    }
}
