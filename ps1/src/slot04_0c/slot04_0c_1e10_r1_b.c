/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801bda50_slot04_0c[];
extern ObjectFn data_801bda54_slot04_0c[];
extern ObjectFn data_801bda5c_slot04_0c[];

void func_80138b38(GameState *state, Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80130678(Object *object, int arg);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_801b39bc_slot04_0c(Object *object);

void func_801b2004_slot04_0c(Object *obj) {
    int i = 0;
    func_80130efc(obj);
    if ((u8)obj->step_flags == 0) {
        if (obj->field_165 != 0) {
            obj->field_165 = 0;
            if (obj->field_4b == 0) {
                obj->other->field_6b = 10;
                i = (obj->field_12a >> 1) + 1;
            }
            obj->field_27b = data_801bda50_slot04_0c[i];
        }
        if (obj->field_0b == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->state_3 = obj->state_3 + 1;
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    }
}

void func_801b20f8_slot04_0c(Object *obj) {
    func_80130efc(obj);
    if (obj->step_flags & 0x80) {
        obj->vel_y = 0x90000;
        obj->accel_y = -0x6000;
        obj->state_3 = obj->state_3 + 1;
    } else {
        if (obj->field_0b == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    }
}

void func_801b21a4_slot04_0c(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->vel_y < 0) {
            obj->state_3 = obj->state_3 + 1;
        }
    }
    func_80130efc(obj);
}

void func_801b2210_slot04_0c(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->field_70) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->field_45 = 0;
        obj->field_159 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
    }
    func_80130efc(obj);
}

void func_801b22a8_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b22ec_slot04_0c(Object *obj) {
    data_801bda54_slot04_0c[obj->state_3](obj);
}

void func_801b232c_slot04_0c(Object *obj) {
    int a = 0x33;
    int s = 3;
    int one = 1;
    int t;
    obj->field_46 = 0x48;
    obj->state_3 = obj->state_3 + 1;
    obj->field_159 = 0;
    obj->field_17b = one;
    obj->field_157 = 0;
    if (obj->field_cd == 0) {
        t = obj->field_c2 & 0x4000;
    } else {
        t = obj->field_129;
    }
    if (t != 0) {
        a = 0x34;
        obj->field_157 = one;
    }
    func_801307e0(obj, a);
    if (obj->field_157 == 0) {
        s = s + 1;
    }
    func_80141f28(obj, s);
    func_80138ae8(&game_state, obj);
}

void func_801b23e8_slot04_0c(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        if (obj->field_157 == 0) {
            func_801312b8(obj);
        } else {
            func_80131468(obj);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b2454_slot04_0c(Object *obj) {
    data_801bda5c_slot04_0c[obj->state_3](obj);
}

void func_801b2494_slot04_0c(Object *obj) {
    int a = 0x2b;
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        a = 0x2f;
    }
    func_80130678(obj, a);
}

void func_801b250c_slot04_0c(Object *obj) {
    int a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_45 != 0) {
            a = 8;
            b = 0x3f;
        }
        func_801495f8(obj, (unsigned)(a << 16) >> 16, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b39bc_slot04_0c(obj);
    }
    func_80130efc(obj);
}

void func_801b25ac_slot04_0c(Object *obj) {
    int a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_46 = (u8)obj->field_46 | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}
