/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
int func_80141b28(Object *object);
int func_80141e34(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void func_80142ba0(Object *object);
void func_80146998(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);

extern ObjectFnInt data_801c732c_slot04_11[];
extern ObjectFn data_801c7354_slot04_11[];

void func_801b0fec_slot04_11(Object *obj);
void func_801b10d8_slot04_11(Object *obj);
int func_801b1324_slot04_11(Object *obj);
int func_801b1358_slot04_11(Object *obj);
void func_801b5ab8_slot04_11(Object *obj);

int func_801b0a84_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if (func_801417cc(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 1;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
        return 1;
    } else {
        return 0;
    }
}

int func_801b0b04_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if (func_801417cc(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 2;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
        return 1;
    } else {
        return 0;
    }
}

int func_801b0b88_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if (func_801417cc(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
        return 1;
    } else {
        return 0;
    }
}

int func_801b0c0c_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if ((u8)func_80141788(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 4;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142718(obj);
        return 1;
    } else {
        return 0;
    }
}

int func_801b0ca4_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if ((u8)func_80141788(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 5;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142718(obj);
        return 1;
    } else {
        return 0;
    }
}

int func_801b0d3c_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if ((u8)func_80141788(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 6;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
        return 1;
    } else {
        return 0;
    }
}

int func_801b0dd4_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_45 == 0) return 0;
    if (!(obj->pos_y < obj->field_70 - 0x18)) return 0;
    if ((u8)func_801418bc(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 7;
        obj->field_159 = 1;
        obj->field_4b = obj->field_25c;
        func_80142778(obj);
        return 1;
    } else {
        return 0;
    }
}

int func_801b0e94_slot04_11(Object *obj) {
    if (*(u16 *)&obj->field_04 != 1) goto c;
    if (obj->field_06 == 5) goto z;
c:
    if (func_801417cc(obj)) goto b;
z:
    obj->flags_28b++;
    return 0;
b:
    obj->field_04 = 1;
    obj->field_06 = 7;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_15a = 8;
    obj->field_0b = obj->field_158;
    return 1;
}

int func_801b0f28_slot04_11(Object *obj) {
    if (obj->field_7e != 0) goto c;
    if (obj->field_177 == 0) goto z;
c:
    if (func_801417cc(obj)) goto b;
z:
    return 0;
b:
    obj->field_04 = 1;
    obj->field_06 = 7;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_15a = 9;
    obj->field_0b = obj->field_158;
    return 1;
}

int func_801b0fb0_slot04_11(Object *obj) {
    if (func_80141b28(obj)) {
        func_801b0fec_slot04_11(obj);
        return 1;
    }
    return 0;
}

void func_801b0fec_slot04_11(Object *obj) {
    int a = 0x10;
    int b = 0x14;

    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 0xa;
    obj->field_159 = 1;
    obj->field_12a = 4;
    obj->field_0b = obj->field_158;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x13;
        b = 0x17;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->field_27b = b;
    func_80146998(obj);
    func_801307e0(obj, 0x1e);
}

int func_801b109c_slot04_11(Object *obj) {
    if (func_80141b28(obj)) {
        func_801b10d8_slot04_11(obj);
        return 1;
    }
    return 0;
}

void func_801b10d8_slot04_11(Object *obj) {
    int a = 0x13;
    int b = 0x17;

    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 0xb;
    obj->field_159 = 1;
    obj->field_12a = 4;
    obj->field_0b = obj->field_158;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x18;
        b = 0x1c;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->field_27b = b;
    func_80146998(obj);
    func_801307e0(obj, 0x1f);
}

int func_801b1188_slot04_11(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_45 == 0) {
        if (!func_80141e34(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->field_04 = 1;
        obj->field_06 = 8;
        obj->field_05 = 0;
        obj->field_07 = 0;
        obj->field_15a = 0xc;
        obj->field_0b = obj->field_158;
        return 1;
    }
    if (obj->field_7e != 0) return 0;
    if (!(u8)func_801418bc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_06 = 8;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_15a = 0xc;
    return 1;
}

void func_801b127c_slot04_11(Object *obj) {
    u8 k;

    if (obj->field_15a < 8) {
        k = 0x11;
        if (obj->field_219 != 0) {
            k = 0x13;
        }
        if (obj->kind != k) {
            obj->kind = k;
            func_801b5ab8_slot04_11(obj);
            select_box_tables(obj);
            build_metrics(obj);
        }
    }
    data_801ad398 = data_801c732c_slot04_11[obj->field_15a](obj);
}

int func_801b1324_slot04_11(Object *obj) {
    return obj->kind == 0x11;
}

int func_801b1338_slot04_11(Object *obj) {
    return func_801b1324_slot04_11(obj);
}

int func_801b1358_slot04_11(Object *obj) {
    return obj->kind == 0x13;
}

int func_801b136c_slot04_11(Object *obj) {
    return func_801b1358_slot04_11(obj);
}

int func_801b138c_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b13bc_slot04_11(Object *obj) {
    if (obj->kind == 0x11) {
        return (s16)obj->field_c6 >= 0x30;
    }
    return 0;
}

int func_801b13e4_slot04_11(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->field_c6 >= 0x30;
    }
    return 0;
}

int func_801b140c_slot04_11(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->field_c6 >= 6;
    }
    return 0;
}

int func_801b1434_slot04_11(Object *obj) {
    return 1;
}

int func_801b143c_slot04_11(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b1448_slot04_11(Object *obj) {
    data_801c7354_slot04_11[obj->field_15a](obj);
}
