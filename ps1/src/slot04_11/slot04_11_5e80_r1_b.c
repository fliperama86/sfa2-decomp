/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7a34_slot04_11[];
extern ObjectFn data_801c7a40_slot04_11[];
extern ObjectFn data_801c7a48_slot04_11[];
extern u8 data_801c7a50_slot04_11[];

void func_80131468(Object *obj);
void func_80131638(Object *obj);
u8 func_80149b80(Object *obj);
u8 func_8013f8c4(Object *obj, int a, int b);
void func_8007bc00_slot16(Object *obj);
void func_8007be8c_slot16(Object *obj);
void func_801b5ab8_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);
void func_801b655c_slot04_11(Object *obj);

void func_801b600c_slot04_11(Object *obj) {
    func_801312b8(obj);
}

void func_801b602c_slot04_11(Object *obj) {
    func_80131468(obj);
}

void func_801b604c_slot04_11(Object *obj) {
    func_80131638(obj);
}

void func_801b606c_slot04_11(Object *obj) {
    if (obj->field_219 == 0) {
        obj->kind = 0x11;
        func_801b5ab8_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_8007bc00_slot16(obj);
    } else {
        data_801c7a34_slot04_11[obj->field_128 >> 1](obj);
    }
}

void func_801b60f4_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7a40_slot04_11[obj->field_07](obj);
}

void func_801b6134_slot04_11(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && func_8013f8c4(obj, -0x17, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b6604_slot04_11(obj);
        func_801b6628_slot04_11(obj);
    }
}

void func_801b61c4_slot04_11(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b66ac_slot04_11(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
        func_801b6628_slot04_11(obj);
    }
}

void func_801b6230_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c7a48_slot04_11[obj->field_07](obj);
}

void func_801b6274_slot04_11(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_129 != 0 && obj->field_12a == 4) {
        obj->field_29c = 1;
    }
    func_801b6604_slot04_11(obj);
    func_801b6628_slot04_11(obj);
}

void func_801b62d8_slot04_11(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b66cc_slot04_11(obj);
    }
    if (func_80149b80(obj) & 0xff) {
        obj->field_07 = 0;
    }
    func_80130efc(obj);
    func_801b6628_slot04_11(obj);
}

void func_801b633c_slot04_11(Object *obj) {
    int n;

    if (obj->field_67 != 0 && *(u8 *)&obj->field_3a != 0 && obj->field_218 != 0) {
        n = 0x18;
        if (obj->field_48 != 0) {
            n = 0x19;
        }
        func_801307e0(obj, n);
    } else if (func_801b6668_slot04_11(obj) < 0 && obj->pos_y >= obj->field_70) {
        func_801209c4(obj);
        func_801b66ec_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b63fc_slot04_11(Object *obj) {
    if (obj->field_219 == 0) {
        obj->kind = 0x11;
        func_801b5ab8_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_8007be8c_slot16(obj);
    } else if (obj->field_211 != 0) {
        func_801b655c_slot04_11(obj);
    } else {
        func_801b6480_slot04_11(obj);
    }
}

void func_801b6480_slot04_11(Object *obj) {
    s16 n;
    s16 t;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80141f28(obj, *(s16 *)((u8 *)data_801c7a50_slot04_11 + (obj->field_12a & 0xfe)));
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
