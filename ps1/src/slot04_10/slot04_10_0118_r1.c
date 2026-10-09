/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6e08_slot04_10[];
extern u8 data_801ba218_slot04_10[];
extern u32 data_801c68bc_slot04_10[];
extern ObjectFn data_801c695c_slot04_10[];
extern ObjectFn data_801c6964_slot04_10[];
extern ObjectFn data_801c696c_slot04_10[];
extern ObjectFn data_801c6974_slot04_10[];
extern ObjectFn data_801c697c_slot04_10[];

Object *func_8011f32c(void);
u8 func_8013f8c4(Object *object, int a, int b);
int func_80141618(Object *object);
int func_801412a4(Object *object);
void func_80130dc0(Object *object);
void func_80131468(Object *object);

void func_801b0414_slot04_10(Object *obj);
void func_801b04ec_slot04_10(Object *obj);
void func_801b0334_slot04_10(Object *obj);
void func_801b0648_slot04_10(Object *obj);
void func_801b02f4_slot04_10(Object *obj);
void func_801b07e4_slot04_10(Object *obj);
void func_801b0688_slot04_10(Object *obj);
void func_801b090c_slot04_10(Object *obj);
void func_801b1078_slot04_10(Object *obj);

void func_801b0118_slot04_10(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b6e08_slot04_10;
    obj->field_9c = data_801ba218_slot04_10;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c68bc_slot04_10[i];
    }
    ref_other.p = func_8011f32c();
    if (ref_other.p != 0) {
        ref_other.p->field_00 = 1;
        ref_other.p->field_02 = 0x10;
        ref_other.p->field_3c = obj;
        ref_other.p->field_0c = obj->field_0c;
        ref_other.p->field_0e = obj->field_0e;
        obj->field_28 = (u32)ref_other.p;
        ref_other.p->field_66 = obj->field_66;
        ref_other.p->field_0d = obj->field_0d;
        ref_other.p->field_7a = obj->field_7a;
        ref_other.p->field_7c = obj->field_7c;
        ref_other.p->field_90 = obj->field_90;
        ref_other.p->field_98 = obj->field_98;
        ref_other.p->field_9c = obj->field_9c;
    }
}

void func_801b029c_slot04_10(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b090c_slot04_10(obj);
    } else if (obj->field_128 != 0) {
        func_801b0648_slot04_10(obj);
    } else {
        func_801b02f4_slot04_10(obj);
    }
}

void func_801b02f4_slot04_10(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b04ec_slot04_10(obj);
    } else {
        func_801b0334_slot04_10(obj);
    }
}

void func_801b0334_slot04_10(Object *obj) {
    data_801c695c_slot04_10[obj->field_07](obj);
}

void func_801b0374_slot04_10(Object *obj) {
    int one;

    obj->field_67 = 0;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && func_8013f8c4(obj, -0x25, 0x14) != 0) {
        func_801b0414_slot04_10(obj);
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        if (obj->field_12a == 4) {
            obj->field_278 = one;
        }
    }
}

void func_801b0414_slot04_10(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 2;
    obj->field_06 = 0;
    obj->field_07 = 0;
}

void func_801b0430_slot04_10(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_801b1078_slot04_10(obj);
    } else {
        if ((t & 1) != 0) {
            s16 d = 3;
            if (obj->field_0b == 0) {
                d = -3;
            }
            obj->pos_x = d + obj->pos_x;
        }
        if ((obj->field_3a & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b04ec_slot04_10(Object *obj) {
    data_801c6964_slot04_10[obj->field_07](obj);
}

void func_801b052c_slot04_10(Object *obj) {
    obj->field_67 = 0;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) == 0) {
        obj->field_159 = 1;
        func_80130dc0(obj);
        return;
    }
    if (obj->field_12a != 0 && func_8013f8c4(obj, -0x25, 0x14) != 0) {
        func_801b0414_slot04_10(obj);
        return;
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b05c4_slot04_10(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_801b1078_slot04_10(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b0648_slot04_10(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b07e4_slot04_10(obj);
    } else {
        func_801b0688_slot04_10(obj);
    }
}

void func_801b0688_slot04_10(Object *obj) {
    data_801c696c_slot04_10[obj->field_07](obj);
}

void func_801b06c8_slot04_10(Object *obj) {
    obj->field_67 = 0;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a == 0) {
        obj->field_159 = 1;
        func_80130dc0(obj);
        return;
    }
    if ((obj->field_130 & 0x8000) != 0) {
        if (func_8013f8c4(obj, -0x25, 0x14) != 0) {
            func_801b0414_slot04_10(obj);
            return;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b0760_slot04_10(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b07e4_slot04_10(Object *obj) {
    data_801c6974_slot04_10[obj->field_07](obj);
}

void func_801b0824_slot04_10(Object *obj) {
    unsigned a;

    obj->field_159 = 1;
    obj->field_07 = obj->field_07 + 1;
    a = obj->field_12a;
    obj->field_67 = 0;
    if (a == 0 || (obj->field_130 & 0x2000) == 0) {
        func_80130dc0(obj);
    } else {
        func_801307e0(obj, (a >> 1) + 0x55);
    }
}

void func_801b0888_slot04_10(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b090c_slot04_10(Object *obj) {
    data_801c697c_slot04_10[obj->field_12f](obj);
}
