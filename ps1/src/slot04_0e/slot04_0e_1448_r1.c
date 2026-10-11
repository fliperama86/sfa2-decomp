/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5f40_slot04_0e[];
extern ObjectFn data_801c5f4c_slot04_0e[];
extern ObjectFn data_801c5f58_slot04_0e[];
extern ObjectFn data_801c5f64_slot04_0e[];
extern ObjectFn data_801c5f6c_slot04_0e[];
extern ObjectFn data_801c5f78_slot04_0e[];
extern ObjectFn data_801c5f80_slot04_0e[];

int func_80141618(Object *object);
int func_801412a4(Object *object);
void func_80130dc0(Object *object);
void func_801b1710_slot04_0e(Object *obj);
void func_801b1850_slot04_0e(Object *obj);

void func_801b1448_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1488_slot04_0e(Object *obj) {
    data_801c5f40_slot04_0e[obj->field_12a >> 1](obj);
}

void func_801b14cc_slot04_0e(Object *obj) {
    data_801c5f4c_slot04_0e[obj->state_3](obj);
}

void func_801b150c_slot04_0e(Object *obj) {
    obj->state_3++;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x21, 7) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0) {
            obj->state_3 = 2;
            obj->field_159 = 1;
            obj->field_157 = 0;
            obj->field_45 = 1;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            func_801307e0(obj, 0x1c);
            return;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b1604_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        if ((((Slot04bObj *)obj)->field_3a & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b1690_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->field_45 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b16d0_slot04_0e(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b1850_slot04_0e(obj);
    } else {
        func_801b1710_slot04_0e(obj);
    }
}

void func_801b1710_slot04_0e(Object *obj) {
    data_801c5f58_slot04_0e[obj->field_12a >> 1](obj);
}

void func_801b1754_slot04_0e(Object *obj) {
    data_801c5f64_slot04_0e[obj->state_3](obj);
}

void func_801b1794_slot04_0e(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b17c4_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_80131468(obj);
    } else {
        if ((((Slot04bObj *)obj)->field_3a & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b1850_slot04_0e(Object *obj) {
    data_801c5f6c_slot04_0e[obj->field_12a >> 1](obj);
}

void func_801b1894_slot04_0e(Object *obj) {
    data_801c5f78_slot04_0e[obj->state_3](obj);
}

void func_801b18d4_slot04_0e(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b1904_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_80131468(obj);
    } else {
        if ((((Slot04bObj *)obj)->field_3a & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b1990_slot04_0e(Object *obj) {
    data_801c5f80_slot04_0e[obj->state_3](obj);
}

void func_801b19d0_slot04_0e(Object *obj) {
    int one;

    *(s32 *)&obj->vel_x = 0xa0000;
    obj->state_3++;
    *(s32 *)&obj->accel_x = 0xffff4000;
    one = 1;
    obj->field_159 = one;
    if (obj->field_0b == 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    func_80130dc0(obj);
    obj->field_278 = one;
}

void func_801b1a50_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    if (obj->field_3a != 0) {
        o->state_3++;
        func_80120554(o, o->field_02, 0x324);
    }
    func_80130efc(o);
}

void func_801b1aa0_slot04_0e(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (o->field_3a == 0) {
        obj->state_3++;
        obj->pos_x_frac = 0;
    }
    func_80130efc(obj);
}

void func_801b1b00_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}
