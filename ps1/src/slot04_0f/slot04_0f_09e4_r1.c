/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c59f8_slot04_0f[];
extern s32 data_801c5a00_slot04_0f[];
extern ObjectFn data_801c5a0c_slot04_0f[];
extern s32 data_801c5a18_slot04_0f[];

void func_80130dc0(Object *object);
void func_80131468(Object *object);

/* functions of other units of this module */
void func_801b4a3c_slot04_0f(Object *obj);

void func_801b0cac_slot04_0f(Object *obj);
void func_801b0d90_slot04_0f(Object *obj);

void func_801b09e4_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int i;

    o->field_159 = 1;
    obj->field_337 = 0;
    o->state_3++;
    if (o->field_130 & 0x2000) {
        func_80130ec0(o);
        func_80141f28(o, o->field_12a >> 1);
        i = o->field_12a >> 2;
        o->field_248 = data_801c59f8_slot04_0f[i];
        o->field_29b = data_801c59f8_slot04_0f[i + 4];
        func_801307e0(o, (o->field_12a >> 1) + 0x23);
    } else {
        o->state_3 = 2;
        obj->field_337 = 0xff;
        func_80130dc0(o);
    }
}

void func_801b0aa8_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b0ac8_slot04_0f(Object *obj) {
    s32 x;
    s32 y = -0x10000;

    obj->state_3++;
    obj->accel_y = 0;
    obj->vel_y = 0;
    x = data_801c5a00_slot04_0f[obj->field_12a >> 1];
    if (obj->field_0b == 0) {
        x = -x;
        y = 0x10000;
    }
    obj->vel_x = x;
    obj->accel_x = y;
    update_sequence(obj);
}

void func_801b0b30_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_3++;
        play_sound_at(obj, obj->side, 0x324);
    }
    update_sequence(obj);
}

void func_801b0b80_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x == 0) {
        obj->state_3++;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x36);
    } else {
        update_sequence(obj);
    }
}

void func_801b0bf0_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b0c34_slot04_0f(Object *obj) {
    obj->state_3 = 3;
    obj->field_159 = 1;
    obj->field_128 = 4;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_129 == 0) {
        func_801b0cac_slot04_0f(obj);
    } else {
        func_801b0d90_slot04_0f(obj);
    }
}

void func_801b0cac_slot04_0f(Object *obj) {
    int a = 0xc;

    if (obj->field_48 != 0) {
        a = 0x12;
    }
    if (!(obj->field_130 & 0x1000)) {
        if (obj->field_130 & 0x4000) {
            if (obj->field_12a == 4) {
                if (obj->pos_y < obj->ground_y - 0x38) {
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 5;
                    obj->state_3 = 0;
                    func_80141f28(obj, 2);
                    func_801307e0(obj, 0x32);
                    return;
                }
            }
        } else if (obj->field_130 & 0x2000) {
            a += 0x1a;
        }
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
    func_80141f28(obj, obj->field_12a >> 1);
}

void func_801b0d90_slot04_0f(Object *obj) {
    int a = 0xf;

    if (obj->field_48 != 0) {
        a = 0x15;
    }
    if (!(obj->field_130 & 0x1000)) {
        if (obj->field_130 & 0x4000) {
            if (obj->ground_y - 0x38 >= obj->pos_y) {
                obj->state_0 = 1;
                obj->state_1 = 0;
                obj->state_2 = 5;
                obj->state_3 = 0;
                func_80141f28(obj, 2);
                func_801307e0(obj, (obj->field_12a >> 1) + 0x33);
                return;
            }
        } else if (obj->field_130 & 0x2000) {
            a += 0x1a;
        }
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
    func_80141f28(obj, obj->field_12a >> 1);
}

void func_801b0e68_slot04_0f(Object *obj) {
    data_801c5a0c_slot04_0f[obj->state_3](obj);
}

void func_801b0ea8_slot04_0f(Object *obj) {
    int i = 0;
    s32 x;

    obj->state_3++;
    if (obj->field_129 != 0) {
        i = obj->field_12a;
    }
    x = data_801c5a18_slot04_0f[i];
    obj->vel_y = data_801c5a18_slot04_0f[i + 1];
    if (obj->field_0b == 0) {
        x = -x;
    }
    obj->vel_x = x;
}
