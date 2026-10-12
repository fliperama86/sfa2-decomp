/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7388_slot04_11[];
extern u8 data_801c7398_slot04_11[];
extern u16 data_801c739c_slot04_11[];
extern ObjectFn data_801c73a4_slot04_11[];
extern s32 data_801c73c0_slot04_11[];
extern u8 data_801c73f0_slot04_11[];
extern u8 data_801c73f8_slot04_11[];
extern u8 data_801c701c_slot04_11[];
extern u8 data_801c709c_slot04_11[];
void func_801b15c4_slot04_11(Object *obj);
void func_801b180c_slot04_11(Object *obj);
void func_801b1850_slot04_11(Object *obj);
void func_801b17c0_slot04_11(Object *obj);
void func_801b1c58_slot04_11(Object *obj);
void func_801b1f54_slot04_11(Object *obj);
void func_801b5874_slot04_11(Object *obj);
void func_80142adc(Object *object);
int func_801b57a8_slot04_11(Object *obj);

void func_801b1488_slot04_11(Object *obj) {
    data_801c7388_slot04_11[obj->state_3](obj);
}

void func_801b14c8_slot04_11(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->field_12d = 0xb4;
    obj->state_3++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    obj->input_matched_flag = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->direction_index;
    func_801307e0(obj, a);
}

void func_801b1544_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode != 0) {
        o->countdown = 0xf;
        o->state_3++;
        if (o->mode_flag_2 == 0) {
            func_801b15c4_slot04_11(o);
        } else {
            o->countdown = data_801c7398_slot04_11[o->input_dir_second];
            func_801b1850_slot04_11(o);
        }
    } else {
        update_sequence(o);
    }
}

void func_801b15c4_slot04_11(Object *obj) {
    s16 x;
    int h;
    u8 s;

    h = (s16)obj->step_flags >> 8;
    if (obj->mode_flag_2 != 0) {
        func_801b1850_slot04_11(obj);
    } else if ((*(u32 *)&game_state.leader_bits & 0xffff00) != 0 || game_state.field_04 != 0 ||
               (func_8012f56c(obj) & 0xff) != 0) {
        func_801b180c_slot04_11(obj);
    } else {
        if ((h & 0x7f) != 0) {
            obj->step_flags = obj->step_flags & 0x80ff;
            func_80142c04(obj);
            func_80138ae8(&game_state, obj);
            if (obj->input_matched_flag == 0) {
                obj->input_matched_flag = 0xff;
            }
        }
        if ((obj->pressed_bits & 0x94) == 0) {
            func_801b17c0_slot04_11(obj);
        } else {
            if ((obj->pressed_bits & 0x80) != 0) {
                x = 0;
            } else if ((obj->pressed_bits & 0x10) != 0) {
                x = 2;
            } else {
                x = 4;
            }
            if (obj->countdown <= data_801c739c_slot04_11[x >> 1]) {
                func_801b17c0_slot04_11(obj);
            } else {
                obj->countdown = 0xf;
                if (obj->direction_index == x) {
                    update_sequence(obj);
                } else {
                    obj->direction_index = x;
                    s = ((Slot04bObj *)obj)->mode;
                    if (obj->field_49 != 0) {
                        x += 0x40;
                    } else {
                        x += 0x21;
                    }
                    func_801307e0(obj, x);
                    obj->sequence = obj->sequence + s;
                    obj->step_flags = obj->sequence->flags;
                    obj->step_timer = obj->sequence->duration;
                    obj->frame = obj->frames + obj->sequence->frame_index;
                    obj->frame_sound_index = obj->frame->sequence_param;
                    obj->sequence_changed_flag = 1;
                }
            }
        }
    }
}

void func_801b17c0_slot04_11(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        func_801b180c_slot04_11(obj);
    }
}

void func_801b180c_slot04_11(Object *obj) {
    int a;

    obj->state_3++;
    obj->active_flag_4 = 0;
    a = 0x22;
    if (obj->field_49 != 0) {
        a = 0x41;
    }
    a += obj->direction_index;
    func_801307e0(obj, a);
}

void func_801b1850_slot04_11(Object *obj) {
    u16 t;

    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) != 0 ||
        (func_8012f56c(obj) & 0xff) != 0) {
        goto tail;
    }
    t = obj->step_flags;
    if (((t >> 8) & 0x7f) != 0 && (s16)obj->step_timer == 1) {
        obj->step_flags = t & 0x80ff;
        func_80142c04(obj);
        func_80138ae8(&game_state, obj);
        if (obj->input_matched_flag == 0) {
            obj->input_matched_flag = 0xff;
        }
        if (obj->input_dir_second != 0) {
            obj->countdown = (s16)obj->countdown - 1;
            if ((s16)obj->countdown == 0) {
                goto tail;
            }
        } else if ((func_8014a170(obj, (u32 *)data_801c701c_slot04_11) & 0xff) != 0) {
            goto tail;
        }
    }
    update_sequence(obj);
    return;
tail:
    obj->state_3++;
    obj->active_flag_4 = 0;
    func_801307e0(obj, obj->direction_index + 0x22);
}

void func_801b1984_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b19f8_slot04_11(Object *obj) {
    data_801c73a4_slot04_11[obj->state_3](obj);
}

void func_801b1a38_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int d;

    o->active_flag_4 = 1;
    o->state_3++;
    func_80141f28(o, 9);
    func_80138ae8(&game_state, o);
    o->active_flag = 0;
    o->step_limit = 0;
    o->vel_x = data_801c73c0_slot04_11[o->direction_index * 2];
    o->accel_x = data_801c73c0_slot04_11[o->direction_index * 2 + 1];
    o->vel_y = data_801c73c0_slot04_11[o->direction_index * 2 + 2];
    o->accel_y = data_801c73c0_slot04_11[o->direction_index * 2 + 3];
    d = 0x10;
    if (o->facing_side == 0) {
        d = -0x10;
    }
    o->pos_x = o->pos_x + d;
    play_side_sound_at(o, obj->field_a6, 5);
    if (o->field_49 == 0) {
        func_801b1f54_slot04_11(o);
    } else {
        func_801307e0(o, (o->direction_index >> 1) + 0x46);
    }
}

void func_801b1b6c_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 0) {
        o->active_state = 1;
        o->state_3++;
        o->step_count_2 = data_801c73f0_slot04_11[o->direction_index & 0xfe];
    }
    update_sequence(o);
}

void func_801b1bcc_slot04_11(Object *obj) {
    if (obj->active_flag != 0) {
        obj->vel_x = 0xc000;
        obj->vel_y = 0x19000;
        obj->accel_x = 0;
        obj->accel_y = -0x1000;
        obj->countdown = 0x1801;
        obj->state_3++;
    }
    if (func_801b57a8_slot04_11(obj) < 0) {
        func_801b1c58_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1c58_slot04_11(Object *obj) {
    int a = -0x6000;

    obj->state_3 = 5;
    if (obj->field_49 != 0) {
        a = -0x12000;
    }
    obj->accel_y = a;
    func_801307e0(obj, 0x3c);
}

void func_801b1c98_slot04_11(Object *o) {
    u8 t;

    if (func_801b57a8_slot04_11(o) < 0 || ((Slot04bObj *)o)->field_47 == 0) {
        func_801b1c58_slot04_11(o);
    } else {
        ((Slot04bObj *)o)->field_47--;
        ((Slot04bObj *)o)->countdown++;
        if (o->active_flag != 0) {
            if (o->mode_flag_2 != 0 ? (((Slot04bObj *)o)->field_47 < 6 &&
                                    (func_8014a170(o, (u32 *)data_801c709c_slot04_11) & 0xff) == 0)
                                 : ((o->pressed_bits & data_801c73f8_slot04_11[o->direction_index >> 1]) != 0)) {
                o->step_limit++;
                t = ((Slot04bObj *)o)->countdown;
                if (t != 0) {
                    t--;
                }
                ((Slot04bObj *)o)->field_47 = t;
                play_side_sound_at(o, o->side, 6);
                ((Slot04bObj *)o)->countdown = 0;
                o->active_flag = 0;
                if (o->step_limit == o->step_count_2) {
                    o->accel_x = -0x2000;
                    o->accel_y = -0x5000;
                    o->state_3++;
                }
                o->vel_x = 0xc000;
                o->vel_y = 0x19000;
                func_801b1f54_slot04_11(o);
            }
        }
        update_sequence(o);
    }
}

void func_801b1dfc_slot04_11(Object *obj) {
    int a;

    if (func_801b57a8_slot04_11(obj) < 0) {
        a = -0x7000;
        obj->state_3++;
        if (obj->field_49 != 0) {
            a = -0x15000;
        }
        obj->accel_y = a;
    }
    update_sequence(obj);
}
