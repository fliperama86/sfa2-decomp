/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801cea2c_slot04_17[];
extern u8 data_801cea34_slot04_17[];

void func_80130dc0(Object *obj);
void func_80131468(Object *obj);
void func_80131638(Object *obj);
void func_801b1448_slot04_17(Object *obj);
void func_801b5a34_slot04_17(Object *obj);
void func_801b6fa0_slot04_17(Object *obj);
void func_801b7500_slot04_17(Object *obj);
void func_801b75dc_slot04_17(Object *obj);
void func_801b7684_slot04_17(Object *obj);
void func_801b76a8_slot04_17(Object *obj);
int func_801b76e8_slot04_17(Object *obj);
void func_801b772c_slot04_17(Object *obj);
void func_801b774c_slot04_17(Object *obj);
void func_801b776c_slot04_17(Object *obj);

void func_801b7244_slot04_17(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b772c_slot04_17(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
        func_801b76a8_slot04_17(obj);
    }
}

void func_801b72b0_slot04_17(Object *obj) {
    obj->field_157 = 1;
    data_801cea2c_slot04_17[obj->field_07](obj);
}

void func_801b72f4_slot04_17(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_129 != 0 && obj->field_12a == 4) {
        obj->field_29c = 1;
    }
    func_801b7684_slot04_17(obj);
    func_801b76a8_slot04_17(obj);
}

void func_801b7358_slot04_17(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b774c_slot04_17(obj);
    }
    if (func_80149b80(obj) & 0xff) {
        obj->field_07 = 0;
    }
    func_80130efc(obj);
    func_801b76a8_slot04_17(obj);
}

void func_801b73bc_slot04_17(Object *obj) {
    int n;

    if (obj->field_67 != 0 && *(u8 *)&obj->field_3a != 0 && obj->field_218 != 0) {
        n = 0x18;
        if (obj->field_48 != 0) {
            n = 0x19;
        }
        func_801307e0(obj, n);
    } else if (func_801b76e8_slot04_17(obj) < 0 && obj->pos_y >= obj->field_70) {
        func_801209c4(obj);
        func_801b776c_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b747c_slot04_17(Object *obj) {
    if (obj->field_219 == 0) {
        obj->kind = 0x11;
        func_801b5a34_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b6fa0_slot04_17(obj);
    } else if (obj->field_211 != 0) {
        func_801b75dc_slot04_17(obj);
    } else {
        func_801b7500_slot04_17(obj);
    }
}

void func_801b7500_slot04_17(Object *obj) {
    s16 n;
    s16 t;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80141f28(obj, *(s16 *)((u8 *)data_801cea34_slot04_17 + (obj->field_12a & 0xfe)));
    n = 0xc;
    if (obj->field_48 != 0) {
        n = 0x12;
    }
    if (obj->field_129 != 0) {
        n += 3;
    }
    t = obj->field_12a >> 1;
    t += n;
    func_801307e0(obj, t);
    if (obj->field_129 != 0 && obj->field_12a == 4) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 5;
        obj->field_07 = 0;
        obj->field_67 = 0;
    }
}

void func_801b75dc_slot04_17(Object *obj) {
    obj->field_129 = 2;
    if (obj->kind != 0x13 || (s16)obj->field_c6 < 0x30 || obj->field_45 != 0 || obj->field_70 - 0x18 < obj->pos_y) {
        func_801b7500_slot04_17(obj);
    } else {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 7;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_801b1448_slot04_17(obj);
    }
}

void func_801b7684_slot04_17(Object *obj) {
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b76a8_slot04_17(Object *obj) {
    u16 t = obj->field_3a;
    int d = t & 0x7f;

    if (d != 0) {
        obj->field_3a = t & 0xff80;
        if (obj->field_0b == 0) {
            d = -d;
        }
        obj->pos_x = obj->pos_x + d;
    }
}

int func_801b76e8_slot04_17(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_4c += obj->field_54;
    return obj->field_50 += obj->field_58;
}

void func_801b772c_slot04_17(Object *obj) {
    func_801312b8(obj);
}

void func_801b774c_slot04_17(Object *obj) {
    func_80131468(obj);
}

void func_801b776c_slot04_17(Object *obj) {
    func_80131638(obj);
}
