/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4e64_slot04_12[];
extern s16 data_801c4e70_slot04_12[];

u8 func_80149b80(Object *object);
void func_80131468(Object *object);
void func_80131638(Object *object);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801b45c4_slot04_12(Object *obj);
void func_801b45f4_slot04_12(Object *obj);
void func_801b465c_slot04_12(Object *obj);
void func_801b4584_slot04_12(Object *obj);
void func_801b49b4_slot04_12(Object *obj);
void func_801b4ac0_slot04_12(Object *obj);
void func_801b4ba8_slot04_12(Object *obj);
int func_801b4bcc_slot04_12(Object *obj);

void func_801b4480_slot04_12(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a == 4) {
        func_801204f4(obj, obj->side, 0xb);
    }
    func_801b4ba8_slot04_12(obj);
}

void func_801b44dc_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4544_slot04_12(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b465c_slot04_12(obj);
    } else {
        func_801b4584_slot04_12(obj);
    }
}

void func_801b4584_slot04_12(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b45c4_slot04_12(obj);
    } else {
        func_801b45f4_slot04_12(obj);
    }
}

void func_801b45c4_slot04_12(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    func_801b4ba8_slot04_12(obj);
}

void func_801b45f4_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b465c_slot04_12(Object *obj) {
    data_801c4e64_slot04_12[obj->field_07](obj);
}

void func_801b469c_slot04_12(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a == 4 && obj->field_219 != 0) {
        obj->field_159 = 1;
        obj->field_07++;
        func_80141f28(obj, 2);
        obj->field_50 = 0x78000;
        obj->field_54 = 0;
        obj->field_58 = -0x5000;
        if (obj->field_0b != 0) {
            obj->field_4c = 0x18000;
        } else {
            obj->field_4c = 0xfffe8000;
        }
        obj->field_45 = 1;
        obj->field_157 = 0;
        obj->pos_y -= 0x10;
        func_801307e0(obj, 0x1b);
    } else {
        func_801b4ba8_slot04_12(obj);
    }
}

void func_801b4770_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b47d8_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (func_801b4bcc_slot04_12(obj) < 0 && obj->field_70 <= obj->pos_y) {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_0b = obj->field_0b ^ 1;
        func_801209c4(obj);
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b485c_slot04_12(Object *obj) {
    int t = 0xc;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    if (obj->field_129 == 0) {
        if (obj->field_12a != 0) {
            if (obj->field_218 != 0 && obj->field_70 - 0x30 < obj->pos_y) {
                if (func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                    obj->field_04 = 1;
                    obj->field_05 = 2;
                    obj->field_06 = 0;
                    obj->field_07 = 0;
                    return;
                }
            }
        }
    } else if (obj->field_12a == 2 && obj->field_219 != 0) {
        func_801b49b4_slot04_12(obj);
        return;
    }
    func_80141f28(obj, data_801c4e70_slot04_12[obj->field_12a >> 1]);
    if (obj->field_48 != 0) {
        t = 0x12;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    func_801307e0(obj, (u16)((obj->field_12a >> 1) + t));
}

void func_801b49b4_slot04_12(Object *obj) {
    int t = 0x1c;

    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    func_80141f28(obj, 1);
    obj->field_12f = 0;
    obj->field_67 = 0;
    if (obj->field_48 != 0) {
        t = 0x1d;
    }
    func_801307e0(obj, t);
}

void func_801b4a24_slot04_12(Object *obj) {
    if (obj->field_12f == 0) {
        if (obj->field_67 != 0) {
            func_801b4ac0_slot04_12(obj);
        } else if (func_801b4bcc_slot04_12(obj) < 0 && obj->pos_y >= obj->field_70) {
            func_801209c4(obj);
            func_80131638(obj);
        } else {
            func_80130efc(obj);
        }
    }
}
