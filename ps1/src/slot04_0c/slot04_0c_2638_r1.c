/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bda6c_slot04_0c[];
extern ObjectFn data_801bda74_slot04_0c[];
extern ObjectFn data_801bda84_slot04_0c[];
extern u8 data_801bdaa4_slot04_0c[];
extern s32 data_801bdaa8_slot04_0c[];

int func_80130184(Object *object);
void func_80131638(Object *object);
void func_80142c70(Object *object);
void func_80142fe8(Object *object);
void func_80141f28(Object *object, short delta);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801b27d4_slot04_0c(Object *object);

void func_801b2638_slot04_0c(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    update_sequence(o);
    o->field_46 = t = (o->field_46 & 0xff00) | ((o->field_46 & 0xff) - 1);
    if ((t & 0xff) == 0) {
        if (o->field_45 != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    } else {
        if (obj->field_134 != 0) {
            if (obj->field_27d == 0) {
                obj->field_27d = obj->field_27c;
            }
        } else if ((t & 0xff00) != 0) {
            o->field_46 = t - 0x100;
        }
    }
    func_80142fe8(o);
}

void func_801b2708_slot04_0c(Object *obj) {
    data_801bda6c_slot04_0c[obj->state_3](obj);
}

void func_801b2748_slot04_0c(Object *obj) {
    obj->field_159 = 0;
    obj->field_17b = 1;
    obj->state_3++;
    func_801307e0(obj, 0x35);
    func_80138ae8(&game_state, obj);
    func_801b27d4_slot04_0c(obj);
}

void func_801b27a4_slot04_0c(Object *obj) {
    update_sequence(obj);
    func_801b27d4_slot04_0c(obj);
}

void func_801b27d4_slot04_0c(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        func_80131638(obj);
    }
}

void func_801b2810_slot04_0c(Object *obj) {
    data_801bda74_slot04_0c[obj->state_3](obj);
}

void func_801b2850_slot04_0c(Object *obj) {
    obj->field_17b = 1;
    obj->field_45 = 1;
    obj->state_3++;
    obj->field_157 = 0;
    if (obj->field_0b == 0) {
        obj->vel_x = -0x40000;
    } else {
        obj->vel_x = 0x40000;
    }
    func_801307e0(obj, 0x2a);
    func_80138ae8(&game_state, obj);
}

void func_801b28c4_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->field_46 = 0x48;
        obj->field_45 = 0;
        obj->state_3++;
        obj->field_0b = obj->field_158;
        func_80141f28(obj, 5);
        func_801307e0(obj, 0x2c);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags != 0) {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
    }
}

void func_801b2960_slot04_0c(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b29ac_slot04_0c(Object *obj) {
    obj->state_3 = 1;
    obj->field_45 = 1;
    obj->field_157 = 0;
    if (obj->field_0b == 0) {
        obj->vel_x = 0x30000;
    } else {
        obj->vel_x = -0x30000;
    }
    func_801307e0(obj, 0x2b);
}

void func_801b29f4_slot04_0c(Object *obj) {
    data_801bda84_slot04_0c[obj->state_3](obj);
}

void func_801b2a34_slot04_0c(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x2d);
}

void func_801b2a98_slot04_0c(Object *o) {
    if ((u8)o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        func_801483a4(o, 3, 0x55);
        play_sound_at(o, o->side, 0x31c);
    }
    update_sequence(o);
}

void func_801b2b18_slot04_0c(Object *o) {    int i = 0;

    update_sequence(o);
    if ((u8)o->step_flags == 0) {
        o->state_3++;
        o->field_165 = 0;
        if (o->field_4b == 0) {
            o->other->field_6b = 0xa;
            i = (o->field_12a >> 1) + 1;
        }
        ((Slot04bObj *)o)->field_27b = data_801bdaa4_slot04_0c[i];
    }
}

void func_801b2ba4_slot04_0c(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
    }
}

void func_801b2bec_slot04_0c(Object *o) {
    u16 a;
    int v;

    update_sequence(o);
    a = o->step_flags;
    if ((a & 0xff) == 2) {
        o->step_flags = (a & 0xff00) | 1;
        if (((Slot04bObj *)o->other)->field_a7 == 0xb) {
            play_side_sound_at(o, o->side, 0x12);
            o->field_4a = 0;
        }
    }
    if ((u8)o->step_flags == 0) {
        o->field_45 = 1;
        o->state_3++;
        v = data_801bdaa8_slot04_0c[o->field_12a >> 1];
        o->vel_y = 0x90000;
        o->accel_x = -0x8000;
        o->accel_y = -0x6000;
        o->vel_x = v;
    }
}

void func_801b2cac_slot04_0c(Object *obj) {
    if (obj->vel_x < 0) {
        obj->state_3++;
    } else {
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->field_0b == 0) {
            *(s32 *)&obj->pos_x_frac -= obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        obj->vel_x += obj->accel_x;
    }
    update_sequence(obj);
}

void func_801b2d58_slot04_0c(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->pos_y < obj->field_70) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->field_45 = 0;
        obj->field_159 = 0;
        obj->state_3++;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
    }
    update_sequence(obj);
}

void func_801b2df0_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
