/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6c6c_slot04_10[];
extern u8 data_801c6c9c_slot04_10[];
extern u16 data_801c6ca4_slot04_10[];
extern Slot04_10Rec6cb0 data_801c6cb0_slot04_10[];

int func_80130184(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_80145f98(Object *object);
void func_80146960(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801b5abc_slot04_10(Object *obj);
void func_801b5dd0_slot04_10(Object *obj);

void func_801b5768_slot04_10(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        update_sequence(o);
    } else {
        o->active_state = 1;
        o->state_3++;
        o->step_flags = o->step_flags & 0xff00;
        ref_other.p = o->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        func_80140770(o, 0x22, 0xf, -0x200, 0, 0, 0);
        ref_other.p = o->other;
        if ((s16)ref_other.p->health < 0) {
            o->field_167 = (o->direction_index >> 1) + 0xc;
        }
        o->vel_x = 0x48000;
        o->vel_y = 0x80000;
        o->accel_y = 0xffff7800;
        o->accel_x = 0;
        if (o->facing_side != 0) {
            o->vel_x = -o->vel_x;
        }
    }
}

void func_801b588c_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = 8;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x33);
    }
}

void func_801b5900_slot04_10(Object *obj) {
    data_801c6c6c_slot04_10[obj->state_3](obj);
}

void func_801b5940_slot04_10(Object *obj) {
    ((Slot04bObj *)obj)->table_index_2 = 0;
    obj->state_3++;
    func_801428e4(obj);
    func_80138b38((GameState *)game_state.config, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x29);
}

void func_801b599c_slot04_10(Object *obj) {
    int t = -1;

    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            t = 1;
        }
        obj->field_165 = t;
        func_801483a4(obj, -0x13, 0x53);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}

void func_801b5a18_slot04_10(Object *obj) {
    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        obj->other_lock = 0;
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 0xa;
        }
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 0;
        func_801307e0(obj, 0x3f);
        func_801b5abc_slot04_10(obj);
    }
}

void func_801b5abc_slot04_10(Object *obj) {
    s16 t = *(u16 *)(data_801c6c9c_slot04_10 + (obj->direction_index & 0xfe));

    if ((u8)func_8013f8c4(obj, -0x26, (s16)(t - 0x26)) == 0) {
        obj->state_3 = 0xa;
        update_sequence(obj);
    } else {
        obj->state_3++;
        if (obj->direction_index != 0) {
            play_side_sound_at(obj, obj->side, 8);
        } else {
            play_side_sound_at(obj, obj->side, 4);
        }
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        func_80145f98(obj);
        func_801307e0(obj, 0x2a);
    }
}

void func_801b5b90_slot04_10(Object *o) {
    s16 t;
    u16 x;
    int one;
    int m;
    int d;

    update_sequence(o);
    t = o->step_flags;
    if (t < 0) {
        if (o->direction_index != 0 && ((Slot04bObj *)o)->table_index_2 == 0) {
            ((Slot04bObj *)o)->table_index_2 = 1;
            func_801307e0(o, 0x2b);
        } else {
            one = 1;
            o->active_state = one;
            ((Slot04bObj *)o)->field_298 = 0;
            o->state_3++;
            func_801307e0(o, 0x2c);
            ((Slot04bObj *)o)->table_index_2 = 0;
            if (o->direction_index == 4) {
                ((Slot04bObj *)o)->table_index_2 = one;
            }
            func_801b5dd0_slot04_10(o);
        }
        return;
    }
    if ((t & 0xff00) != 0) {
        if ((t & 0xff00) != 0x100) {
            o->step_flags = t & 0xff;
            if (o->direction_index == 2) {
                play_side_sound_at(o, o->side, 4);
            }
        } else {
            o->step_flags = t & 0xff;
            if (o->direction_index == 0) {
                play_side_sound_at(o, o->side, 5);
            }
        }
    }
    x = o->step_flags;
    if ((x & 0xff) == 0) {
        return;
    }
    if ((x & 0xff) == 1) {
        m = -0x100;
        func_80140cd8(o, (s16)(*(u16 *)((u8 *)data_801c6ca4_slot04_10 + ((((Slot04bObj *)o)->table_index_2 + o->direction_index) << 1)) | m), 0);
        o->step_flags = o->step_flags & m;
        game_state.config->setup_value = 0x3c;
        play_sound_at(o, o->side, 0x319);
        if (o->direction_index != 0) {
            if (((Slot04bObj *)o)->table_index_2 == 0) {
                if (o->direction_index != 4) {
                    return;
                }
                play_side_sound_at(o, o->side, 4);
            } else {
                play_side_sound_at(o, o->side, 5);
            }
        }
        func_80146960(o);
    } else if ((x & 0xff) == 2) {
        d = 0x58;
        o->step_flags = x & 0xff00;
        if (o->facing_side != 0) {
            d = -0x58;
        }
        o->pos_x = o->pos_x - d;
    } else if ((x & 0xff) == 3) {
        o->step_flags = x & 0xff00;
        if (o->direction_index == 0) {
            func_801307e0(o, 0x2b);
        }
    }
}

void func_801b5dd0_slot04_10(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[4];

    obj->vel_y = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->table_index_2].a;
    obj->accel_y = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->table_index_2].b;
    obj->field_f4 = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->table_index_2].c;
    obj->vel_x = 0;
    obj->accel_x = 0;
}

void func_801b5e48_slot04_10(Object *obj) {
    func_80130184(obj);
    if (obj->vel_y >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->accel_y = obj->field_f4;
        update_sequence(obj);
    }
}

void func_801b5e9c_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        update_sequence(obj);
    } else if (obj->direction_index == 4 && ((Slot04bObj *)obj)->table_index_2 != 2) {
        obj->state_3 = 0xb;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff00;
        func_801307e0(obj, 0x2d);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801307e0(obj, 0x2e);
    }
}
