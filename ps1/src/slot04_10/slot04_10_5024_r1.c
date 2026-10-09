/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;

void func_801b4ec0_slot04_10(Object *obj);
void func_801b1078_slot04_10(Object *obj);
void func_801b1098_slot04_10(Object *obj);
int func_80130184(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);

void func_801b5024_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (obj->field_50 >= 0) {
            func_801b4ec0_slot04_10(obj);
        }
        func_80130efc(obj);
    } else {
        obj->field_07++;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->field_260 = 1;
        func_80140770(obj, 0x1f, 5, 0x16, 0x1a, 1, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
            data_80190468.p->field_6b = 0;
            func_80147000(obj);
        }
        obj->field_4c = 0x10000;
        if (obj->field_0b == 0) {
            obj->field_4c = -0x10000;
        }
        if (obj->field_50 >= 0) {
            obj->field_50 = 0;
        }
        obj->field_0b ^= 1;
        func_801307e0(obj, 0x32);
    }
}

void func_801b5174_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj)) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->field_159 = 0;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x33);
    }
}

void func_801b51f4_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b1078_slot04_10(obj);
    }
}

void func_801b5250_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (obj->field_50 >= 0) {
            func_801b4ec0_slot04_10(obj);
        }
        func_80130efc(obj);
    } else {
        obj->field_58 = 0xffff6000;
        obj->field_07++;
        obj->field_4c = 0;
        obj->field_54 = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
        }
    }
}

void func_801b52e0_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj)) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x35);
    }
}

void func_801b535c_slot04_10(Object *obj) {
    u16 v = obj->field_3a;
    u8 st = v;
    if (st == 0) {
        func_80130efc(obj);
    } else if (st == 0xf) {
        obj->field_3a = v & 0xff00;
        data_80190468.p->field_63 = (obj->field_12a << 2) + 0x3c;
        func_80146960(obj);
        func_80130efc(obj);
        func_801204f4(obj, obj->side, 0xa);
        if ((u8)func_80140cd8(obj, 0xd, 0x1b)) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        }
        func_80120554(obj, obj->side, 0x319);
    } else {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_3a &= 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->field_260 = 1;
        func_80140770(obj, 0x23, 0xf, -0x200, 0, 0, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
        }
        obj->field_4c = 0x48000;
        obj->field_50 = 0x90000;
        obj->field_54 = 0;
        obj->field_58 = 0xffff7800;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
        }
    }
}

void func_801b5528_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj)) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x36);
    }
}

void func_801b55a4_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->field_0b ^= 1;
        func_801b1098_slot04_10(obj);
    }
}

void func_801b5610_slot04_10(Object *obj) {
    func_801b4ec0_slot04_10(obj);
    if (obj->field_50 < 0) {
        obj->field_58 = -0x8000;
        obj->field_07++;
        obj->field_4c = 0;
        obj->field_54 = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
        }
    }
    func_80130efc(obj);
}

void func_801b5680_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj)) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        data_80190468.p->field_63 = (obj->field_12a << 2) + 0x3c;
        func_80146960(obj);
        func_801307e0(obj, 0x38);
        if ((u8)func_80140cd8(obj, 0xa, 0x1c)) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        }
        func_80120554(obj, obj->side, 0x319);
    }
}
