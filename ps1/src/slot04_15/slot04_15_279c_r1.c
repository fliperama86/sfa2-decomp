/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2e8c_slot04_15[];
extern u8 data_801c2ea4_slot04_15[];
extern u8 data_801c2ea8_slot04_15[];
extern ObjectFn data_801c3114_slot04_15[];
extern ObjectFn data_801c3128_slot04_15[];
extern ObjectFn data_801c3130_slot04_15[];

void func_801483a4(Object *object, int a, int b);
void set_position_from_other_box(Object *object);
void func_801428e4(Object *object);
void func_80145d20(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80140fe0(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
int func_801410c8(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80130678(Object *object, int index);
void func_801b33c8_slot04_15(Object *object);
void func_801b2d48_slot04_15(Object *obj);
void func_801b2ea4_slot04_15(Object *obj);
void func_801b303c_slot04_15(Object *obj);
void func_801b30c8_slot04_15(Object *obj, s16 a);

void func_801b279c_slot04_15(Object *obj) {
    int i;
    u8 t;
    u16 a;
    s32 *tbl = data_801c2e8c_slot04_15;

    t = obj->step_flags;
    obj->field_249 = 2;
    update_sequence(obj);
    if ((t & 0x80) != 0) {
        i = obj->field_12a;
        obj->accel_x = -0x8000;
        obj->accel_y = 0x6000;
        obj->state_3++;
        obj->vel_x = tbl[i];
        obj->vel_y = -tbl[i + 1];
    } else {
        a = obj->step_flags;
        i = a & 0xff;
        if (i == 2) {
            obj->step_flags = a & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        if ((u8)obj->step_flags == 0) {
            i = 0;
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = obj->field_12a >> 1;
                    i += 1;
                }
                ((Slot04bObj *)obj)->field_27b = data_801c2ea4_slot04_15[i];
            }
            i = obj->vel_x;
            if (obj->field_0b == 0) {
                i = -i;
            }
            *(s32 *)&obj->pos_x_frac = i + *(s32 *)&obj->pos_x_frac;
            obj->vel_x = obj->vel_x + obj->accel_x;
            if (obj->vel_x < 0) {
                obj->vel_x = 0;
                obj->accel_x = 0;
            }
        }
    }
}

void func_801b2900_slot04_15(Object *obj) {
    int v;

    obj->field_249 = 2;
    if (obj->vel_x >= 0) {
        if ((u8)obj->step_flags == 0) {
            obj->field_45 = 1;
            *(s32 *)&obj->pos_y_frac += obj->vel_y;
            obj->vel_y += obj->accel_y;
        }
        v = obj->vel_x;
        if (obj->field_0b == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b29ac_slot04_15(Object *obj) {
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y > 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b2a04_slot04_15(Object *obj) {
    s16 y;

    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    y = obj->ground_y;
    if (obj->pos_y >= y) {
        obj->field_45 = 0;
        obj->pos_y = y;
        obj->pos_y_frac = 0;
        obj->field_159 = 0;
        obj->state_3++;
        func_801209c4(obj);
    } else if ((u8)obj->step_flags != 0) {
        return;
    }
    update_sequence(obj);
}

void func_801b2aa4_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b2ae8_slot04_15(Object *obj) {
    data_801c3114_slot04_15[obj->state_3](obj);
}

void func_801b2b28_slot04_15(Object *obj) {
    obj->field_157 = 1;
    obj->state_3++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x3b);
}

void func_801b2b90_slot04_15(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        int a = -1;

        obj->state_3++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x1a, 0x44);
    }
}

void func_801b2c04_slot04_15(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        u8 i = 0;

        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        ((Slot04bObj *)obj)->field_27b = data_801c2ea8_slot04_15[i];
        obj->vel_x = 0x30000;
        obj->accel_x = 0;
    }
}

void func_801b2c94_slot04_15(Object *obj) {
    u16 a;

    update_sequence(obj);
    a = obj->step_flags;
    if ((a & 0xff) != 0) {
        obj->step_flags = a & 0xff00;
        obj->state_3++;
        obj->accel_x = -0x4000;
    }
    func_801b2d48_slot04_15(obj);
}

void func_801b2cf0_slot04_15(Object *obj) {
    update_sequence(obj);
    if ((s16)obj->step_flags >= 0) {
        func_801b2d48_slot04_15(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b2d48_slot04_15(Object *obj) {
    int v = obj->vel_x;

    if (v >= 0) {
        if (obj->field_0b == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
}

void func_801b2d94_slot04_15(Object *obj) {
    data_801c3128_slot04_15[obj->state_3](obj);
}

void func_801b2dd4_slot04_15(Object *obj) {
    obj->field_17b = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_157 = 0;
    func_801307e0(obj, 0x1c);
}

void func_801b2e24_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2e64_slot04_15(Object *obj) {
    if (obj->field_129 != 0) {
        func_801b33c8_slot04_15(obj);
    } else {
        func_801b2ea4_slot04_15(obj);
    }
}

void func_801b2ea4_slot04_15(Object *obj) {
    data_801c3130_slot04_15[obj->state_3](obj);
}

void func_801b2ee4_slot04_15(Object *obj) {
    obj->gauge = 0xb4;
    obj->field_46 = 0;
    ((Slot04bObj *)obj)->field_1cd = 0;
    obj->state_3++;
    func_80141f28(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x18);
    set_position_from_other_box(obj);
}

void func_801b2f54_slot04_15(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;
    u16 t;
    int k;

    func_80140fe0(obj);
    update_sequence(obj);
    set_position_from_other_box(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        obj->field_45 = 1;
        play_side_sound_at(obj, obj->side, 0x15);
        func_80146478(obj, 1, 0, 0x4e);
        k = 0;
        if (o->field_1cd == 0) {
            o->field_1cd = 0xff;
            k = 6;
        }
        if ((u8)func_80140cd8(obj, k, 0) != 0) {
            play_side_sound_at(obj, obj->side, 0x16);
            func_801b30c8_slot04_15(obj, -0x200);
        } else {
            o->field_46 = o->field_46 + 1;
            func_801b303c_slot04_15(obj);
        }
    } else {
        func_801b303c_slot04_15(obj);
    }
}

void func_801b303c_slot04_15(Object *obj) {
    if (*(u8 *)&obj->field_46 != 0) {
        if (obj->other->field_15b == 0) {
            func_801b30c8_slot04_15(obj, 2);
        } else if ((u8)func_801410c8(obj) != 0) {
            obj->state_3 = obj->state_3 + 1;
            func_801307e0(obj, 0x19);
            set_position_from_other_box(obj);
        }
    }
}

void func_801b30c8_slot04_15(Object *obj, s16 a) {
    int x = 0x40000;
    int t;

    obj->state_3 = 6;
    t = obj->field_0b ^ 1;
    obj->field_0b = t;
    if (t != 0) {
        x = -0x40000;
    }
    obj->vel_x = x;
    obj->vel_y = -0x40000;
    obj->accel_y = 0x6000;
    func_80140770(obj, 0x1e, 0xf, a, 0, 0, 0);
    func_80130678(obj, 0x14);
}
