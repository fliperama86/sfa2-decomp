/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);
extern ObjectFn data_801be1f0_slot04_0c[];
extern ObjectFn data_801be1f8_slot04_0c[];
extern ObjectFn data_801be204_slot04_0c[];
extern ObjectFn data_801be20c_slot04_0c[];

void func_8011f38c(Object *o);
void func_801b4150_slot04_0c(Object *obj);
void func_801b4190_slot04_0c(Object *obj);
void func_801b42c8_slot04_0c(Object *obj);
void func_801b445c_slot04_0c(Object *obj);
void func_80130dc0(Object *obj);

void func_801b40c8_slot04_0c(Object *obj, Object *parent) {
    obj->state_0++;
}

void func_801b40dc_slot04_0c(Object *o, Object *p) {
    if ((s32)o == p->field_28) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}

void func_801b4110_slot04_0c(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b4150_slot04_0c(obj);
    } else {
        func_801b445c_slot04_0c(obj);
    }
}

void func_801b4150_slot04_0c(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b4190_slot04_0c(obj);
    } else {
        func_801b42c8_slot04_0c(obj);
    }
}

void func_801b4190_slot04_0c(Object *obj) {
    data_801be1f0_slot04_0c[obj->state_3](obj);
}

void func_801b41d0_slot04_0c(Object *obj) {
    obj->state_3++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && (u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b4260_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b42c8_slot04_0c(Object *obj) {
    data_801be1f8_slot04_0c[obj->state_3](obj);
}

void func_801b4308_slot04_0c(Object *obj) {
    int one = 1;
    obj->state_3++;
    obj->field_159 = one;
    obj->field_0b = obj->field_158;
    if (obj->field_12a == 4) {
        obj->state_3 = 2;
        func_80130dc0(obj);
        obj->field_278 = one;
    } else {
        func_80130dc0(obj);
    }
}

void func_801b4378_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b43e0_slot04_0c(Object *obj) {
    s16 v = obj->step_flags;
    if (v & 0x8000) {
        func_801312b8(obj);
    } else {
        if (v & 0xff) {
            obj->step_flags = v & 0xff00;
            if (obj->field_0b == 0) {
                *(s32 *)&obj->pos_x_frac += -0x80000;
            } else {
                *(s32 *)&obj->pos_x_frac += 0x80000;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b445c_slot04_0c(Object *obj) {
    obj->field_157 = 1;
    data_801be204_slot04_0c[obj->state_3](obj);
}

void func_801b44a0_slot04_0c(Object *obj) {
    obj->field_159 = 1;
    obj->state_3++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b44d8_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4540_slot04_0c(Object *obj) {
    int t = 0xc;
    obj->state_3 = 3;
    obj->field_159 = 1;
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_48 != 0) {
        t = 0x12;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + t));
}

void func_801b45cc_slot04_0c(Object *obj) {
    data_801be20c_slot04_0c[obj->state_0](obj);
}
