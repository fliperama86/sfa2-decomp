/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2a28_slot04_0d[];
extern ObjectFn data_801c2a38_slot04_0d[];
extern ObjectFn data_801c2a40_slot04_0d[];
extern s32 data_801c2774_slot04_0d[];

void func_80146998(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142adc(Object *object);
void func_801b3b6c_slot04_0d(Object *object);
void func_801b19f8_slot04_0d(Object *object);
void func_801b2018_slot04_0d(Object *object);

void func_801b182c_slot04_0d(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_45 == 0) {
            a = 1;
            b = 0x49;
        } else {
            a = -5;
            b = 0x44;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3b6c_slot04_0d(obj);
    }
    update_sequence(obj);
}

void func_801b18d0_slot04_0d(Object *obj) {
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
    update_sequence(obj);
}

void func_801b195c_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    update_sequence(o);
    o->field_46 = t = (o->field_46 & 0xff00) | ((o->field_46 & 0xff) - 1);
    if ((t & 0xff) != 0) {
        func_801b19f8_slot04_0d(o);
    } else {
        if (o->field_45 != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    }
}

void func_801b19f8_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = obj->field_27c;
        }
    } else if (*(u8 *)&o->field_46 != 0) {
        (*(u8 *)&o->field_46)--;
    }
    func_80142fe8(o);
}

void func_801b1a64_slot04_0d(Object *obj) {
    data_801c2a28_slot04_0d[obj->state_3](obj);
}

void func_801b1aa4_slot04_0d(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801307e0(obj, 0x2f);
    }
}

void func_801b1b14_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    if (obj->field_3a != 0) {
        o->field_45 = 1;
        o->state_3++;
        o->vel_x = 0x18000;
        o->vel_y = 0xfff88000;
        o->accel_y = 0x6000;
    }
}

void func_801b1b7c_slot04_0d(Object *obj) {
    int d;

    func_801b2018_slot04_0d(obj);
    if (obj->field_70 > obj->pos_y) {
        d = obj->vel_x;
        if (obj->field_0b == 0) {
            d = -d;
        }
        *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x26);
    }
}

void func_801b1c18_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1c5c_slot04_0d(Object *obj) {
    data_801c2a38_slot04_0d[obj->state_3](obj);
}

void func_801b1c9c_slot04_0d(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801307e0(obj, 0x30);
    }
}

void func_801b1d0c_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b1d4c_slot04_0d(Object *obj) {
    data_801c2a40_slot04_0d[obj->state_3](obj);
}

void func_801b1d8c_slot04_0d(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    if (obj->field_0b != 0) {
        obj->vel_x = 0x60000;
    } else {
        obj->vel_x = -0x60000;
    }
    a = 0x23;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

void func_801b1e20_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 *t;
    s32 *p;
    u8 g;

    t = data_801c2774_slot04_0d;
    update_sequence(o);
    g = obj->field_3a;
    if (g & 0x80) {
        *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    } else if (g != 0) {
        o->field_45 = 1;
        o->step_flags = o->step_flags & 0xff00;
        o->state_3++;
        p = t + o->field_12a * 2;
        o->vel_x = p[0];
        o->vel_y = p[1];
        o->accel_x = p[2];
        o->accel_y = p[3];
        o->vel_y = -o->vel_y;
        o->accel_y = -o->accel_y;
    }
}

void func_801b1ef8_slot04_0d(Object *obj) {
    int d;

    func_801b2018_slot04_0d(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x26);
    } else {
        if (obj->field_0b != 0) {
            d = obj->vel_x;
        } else {
            d = -obj->vel_x;
        }
        *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
        update_sequence(obj);
    }
}

void func_801b1fbc_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}
