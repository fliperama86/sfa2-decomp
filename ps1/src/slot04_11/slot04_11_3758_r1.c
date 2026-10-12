/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7568_slot04_11[];
extern ObjectFn data_801c75d0_slot04_11[];
extern s32 data_801c7590_slot04_11[];
extern s16 data_801c75c0_slot04_11[];
extern u8 data_801c75fc_slot04_11[];
extern u8 data_801c75fd_slot04_11[];
extern s32 data_801c7604_slot04_11[];
extern ObjectRef data_80190468;

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_80146960(Object *object);
void set_position_from_other_box(Object *object);
int func_8013fd98(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801483a4(Object *object, int a, int b);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b5874_slot04_11(Object *obj);
int func_801b57a8_slot04_11(Object *obj);
int func_801b5764_slot04_11(Object *obj);

void func_801b3758_slot04_11(Object *obj) {
    int a = obj->vel_x;

    if (obj->facing_side == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s32)obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b37f8_slot04_11(Object *obj) {
    obj->field_167 = (obj->direction_index >> 1) + 0xc;
    if (obj->step_limit == 0) {
        obj->step_limit = 0xff;
        data_80190468.p->field_6b = 5;
        func_80147000(obj);
    }
}

void func_801b3844_slot04_11(Object *obj) {
    data_801c7568_slot04_11[obj->state_3](obj);
}

void func_801b3884_slot04_11(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x40);
}

void func_801b38e0_slot04_11(Object *obj) {
    int a;

    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        obj->facing_side = obj->saved_facing;
        obj->pos_x_frac = 0;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_801483a4(obj, 0xc, 0x4d);
        play_sound_at(obj, obj->side, 0x31c);
        play_side_sound_at(obj, obj->side, 0xa);
    }
}

void func_801b3970_slot04_11(Object *obj) {
    int i;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        obj->state_3++;
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
        }
        i = obj->direction_index;
        obj->other_lock = 0;
        obj->field_165 = 0;
        obj->vel_x = data_801c7590_slot04_11[i * 2];
        obj->accel_x = data_801c7590_slot04_11[i * 2 + 1];
        obj->vel_y = data_801c7590_slot04_11[i * 2 + 2];
        obj->accel_y = data_801c7590_slot04_11[i * 2 + 3];
        obj->active_state = 1;
    }
}

void func_801b3a50_slot04_11(Object *obj) {
    if (func_801b57a8_slot04_11(obj) < 0) {
        obj->state_3 = 6;
        obj->variant_index = 0xff;
        func_801307e0(obj, 0x41);
    } else {
        ref_other.p = obj->other;
        if (ref_other.p->field_163 == 0) {
            if ((u8)func_8013fd98(obj, -0x20, 0x20, 0x1a, 0x1a) != 0) {
                obj->state_3++;
                play_sound_at(obj, obj->other->side, 0x31a);
                play_side_sound_at(obj, obj->side, 0xd);
                func_801307e0(obj, obj->direction_index + 0x43);
            }
        }
        update_sequence(obj);
    }
}

void func_801b3b34_slot04_11(Object *obj) {
    if ((u8)obj->step_flags == 0 && func_801b57a8_slot04_11(obj) < 0) {
        obj->accel_y = -0x3800;
        if (obj->ground_y - 8 <= obj->pos_y) {
            obj->state_3++;
            obj->pos_y_frac = 0;
            obj->pos_y = (u16)((Slot04bObj *)obj)->ground_y - 8;
            obj->facing_side = obj->facing_side ^ 1;
            func_801307e0(obj, obj->direction_index + 0x44);
            return;
        }
    }
    update_sequence(obj);
}

void func_801b3bd8_slot04_11(Object *obj) {
    int k = obj->step_flags & 0x7f;
    int j;
    int a;

    if (k != 0) {
        data_80190468.p->vec_table_index = 0x20;
        obj->step_flags = obj->step_flags & 0xff80;
        func_80146960(obj);
        play_side_sound_at(obj, obj->side, 8);
        j = k - 1;
        if (func_80140cd8(obj, data_801c75c0_slot04_11[j * 2], data_801c75c0_slot04_11[j * 2 + 1]) != 0) {
            play_sound_at(obj, obj->side, 0x319);
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
    }
    if (obj->step_flags & 0x80) {
        obj->state_3++;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        func_80140770(obj, 0, 0xa, -0x200, 0, 0, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        a = 0x40000;
        if (obj->facing_side != 0) {
            a = -0x40000;
        }
        obj->vel_y = 0x60000;
        obj->vel_x = a;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
        obj->variant_index = 0;
    } else {
        update_sequence(obj);
        set_position_from_other_box(obj);
    }
}

void func_801b3d94_slot04_11(Object *obj) {
    int r;

    if (obj->variant_index == 0) {
        r = func_801b5764_slot04_11(obj);
    } else {
        r = func_801b57a8_slot04_11(obj);
    }
    if (r < 0 && obj->pos_y >= obj->ground_y) {
        obj->state_3++;
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801307e0(obj, 0x42);
    } else {
        update_sequence(obj);
    }
}

void func_801b3e40_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3ea0_slot04_11(Object *obj) {
    data_801c75d0_slot04_11[obj->state_3](obj);
}

void func_801b3ee0_slot04_11(Object *obj) {
    obj->state_3++;
    obj->field_225 = 1;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_49 == 0) {
        func_80145d20(obj);
    }
    func_801307e0(obj, (obj->direction_index >> 1) + 0x49);
}

void func_801b3f58_slot04_11(Object *obj) {
    int a;

    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_801483a4(obj, -0x35, 0x37);
        play_sound_at(obj, obj->side, 0x31c);
    }
}

void func_801b3fcc_slot04_11(Object *obj) {
    int i;
    int a;
    int b;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        i = 6;
        obj->state_3++;
        obj->step_count_2 = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            i = obj->direction_index;
        }
        obj->step_limit = data_801c75fc_slot04_11[i];
        obj->other_lock = data_801c75fd_slot04_11[i];
        a = data_801c7604_slot04_11[i * 2];
        b = data_801c7604_slot04_11[i * 2 + 1];
        obj->vel_y = data_801c7604_slot04_11[i * 2 + 2];
        obj->accel_y = data_801c7604_slot04_11[i * 2 + 3];
        if (obj->facing_side == 0) {
            a = -a;
            b = -b;
        }
        obj->vel_x = a;
        obj->accel_x = b;
        *(u16 *)&((Slot04bObj *)obj)->input_bits = 0;
    }
}
