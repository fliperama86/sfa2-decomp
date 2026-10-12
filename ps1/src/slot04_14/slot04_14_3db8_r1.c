/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c63c4_slot04_14[];
extern ObjectFn data_801c63cc_slot04_14[];
extern ObjectFn data_801c63d4_slot04_14[];
extern ObjectFn data_801c6420_slot04_14[];
extern s32 data_801c646c_slot04_14[];

void func_80145e70(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801483a4(Object *object, int a, int b);
void func_80126130(void);
void func_80126244(void);
void func_80155d4c(int idx, int side);
void func_80155eac(int idx, int side);
void func_80140598(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, int unused);
void func_80131468(Object *object);
u8 func_801b61e8_slot04_14(Object *obj);
void func_801b3fec_slot04_14(Object *obj);
void func_801b41bc_slot04_14(Object *obj);

void func_801b3db8_slot04_14(Object *obj) {
    obj->direction_index = 4;
    obj->field_255 = 4;
    obj->active_flag_3 = 0;
    obj->state_3++;
    obj->countdown = (u8)obj->countdown | 0x3200;
    obj->variant_index = obj->facing_side;
    func_80145e70(obj);
    func_80141f28(obj, -0x90);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x3a);
}

void func_801b3e38_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        obj->vel_x = -0x80000;
        obj->countdown = (obj->countdown & 0xff00) | 0x30;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x80000;
        }
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x10, 0x58);
    }
    update_sequence(obj);
}

void func_801b3ee4_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if (obj->countdown & 0x8000) {
        obj->field_165 = 0;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xc);
    }
    update_sequence(obj);
}

void func_801b3f44_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) != 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        if (((obj->bound_push_flag >> obj->variant_index) & 1) == 0) {
            if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
                func_801b3fec_slot04_14(obj);
            } else {
                update_sequence(obj);
            }
            return;
        }
    }
    func_801312b8(obj);
}

void func_801b3fec_slot04_14(Object *obj) {
    obj->state_3++;
    obj->countdown = (obj->countdown & 0xff00) | 0x10;
    play_sound_at(obj->other, obj->other->side, 0x31a);
    func_801307e0(obj, 0x3b);
}

void func_801b4048_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) == 0) {
        obj->state_3++;
        obj->countdown = (obj->countdown & 0xff00) | 0x3b;
        update_sequence(obj);
    }
}

void func_801b409c_slot04_14(Object *obj) {
    Object *p = obj->other;

    func_80126130();
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) == 0) {
        obj->state_3++;
        play_sound_at(p, p->side, 0x306);
        func_80126244();
        func_801b41bc_slot04_14(obj);
    } else if (func_801b61e8_slot04_14(obj) != 0) {
        if (obj->mode_flag_2 == 0) {
            obj->sound_index = 0;
            obj->field_bf = 0xff;
            func_80155d4c(4, (s8)obj->side);
            func_80155eac(4, obj->side);
        }
        p->round_count++;
    }
}

void func_801b4178_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b41bc_slot04_14(Object *obj) {
    Object *p = obj->other;

    p->field_15b = 1;
    func_80140598(obj, 0, 3, 0x1f, 0x1c, 0, 0);
    if ((s16)p->health < 0) {
        obj->field_167 = 0xf;
        game_state.spawn_kind = 6;
        func_80147000(obj);
    }
}

void func_801b4240_slot04_14(Object *obj) {
    data_801c63c4_slot04_14[obj->state_3](obj);
}

void func_801b4280_slot04_14(Object *obj) {
    obj->field_157 = 0;
    obj->active_flag_3 = 0;
    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    func_801307e0(obj, 0x3f);
}

void func_801b42cc_slot04_14(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            game_state.state_param = 0x18;
            play_side_sound_at(obj, obj->side, 0xe);
        }
        update_sequence(obj);
    }
}

void func_801b4348_slot04_14(Object *obj) {
    data_801c63cc_slot04_14[obj->state_3](obj);
}

void func_801b4388_slot04_14(Object *obj) {
    obj->active_flag_4 = 1;
    obj->active_state = 1;
    obj->state_3 = 1;
    obj->field_29c = 2;
    obj->active_flag_3 = 0;
    func_80141f28(obj, 1);
    func_80138ae8(&game_state, obj);
    obj->vel_x = -0x40000;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + 0x42);
}

void func_801b440c_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        func_80131468(obj);
    } else {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        update_sequence(obj);
    }
}

void func_801b4464_slot04_14(Object *obj) {
    data_801c63d4_slot04_14[obj->state_3](obj);
}

void func_801b44a4_slot04_14(Object *obj) {
    data_801c6420_slot04_14[obj->state_3](obj);
}

void func_801b44e4_slot04_14(Object *obj) {
    int one = 1;
    int a;

    obj->active_flag_4 = one;
    obj->state_3 = one;
    obj->field_157 = 0;
    func_80141f28(obj, 1);
    func_80138ae8(&game_state, obj);
    a = 0x49;
    if (obj->field_49 != 0) {
        a = 0x6c;
        obj->field_225 = one;
    }
    func_801307e0(obj, a);
}

void func_801b4558_slot04_14(Object *obj) {
    int k;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3++;
        k = obj->direction_index << 1;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->vel_x = data_801c646c_slot04_14[(u8)k];
        obj->accel_x = data_801c646c_slot04_14[(u8)(k | 1)];
        obj->vel_y = data_801c646c_slot04_14[(u8)(k + 2)];
        obj->accel_y = data_801c646c_slot04_14[(u8)(k + 3)];
    }
}

void func_801b4634_slot04_14(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->pos_y = obj->pos_y - 0x28;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->countdown = (u8)obj->countdown | 0xa00;
    }
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac -= obj->vel_x;
    }
    obj->vel_x += obj->accel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}
