/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4c78_slot04_12[];
extern ObjectFn data_801c4c80_slot04_12[];
extern ObjectFn data_801c4c90_slot04_12[];
extern ObjectFn data_801c4c9c_slot04_12[];
extern u16 box_margin;

void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80130678(Object *object, int index);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80131638(Object *object);
void func_801b34f0_slot04_12(Object *obj);
void func_801b353c_slot04_12(Object *obj);
void func_801b3748_slot04_12(Object *obj);
void func_801b35a0_slot04_12(Object *obj);
void func_801b3a08_slot04_12(Object *obj);
int func_801b3a4c_slot04_12(Object *obj);

void func_801b30d0_slot04_12(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b3130_slot04_12(Object *obj) {
    data_801c4c78_slot04_12[obj->field_07](obj);
}

void func_801b3170_slot04_12(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    func_801307e0(obj, 0x20);
}

void func_801b31bc_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3200_slot04_12(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b3260_slot04_12(Object *obj) {
    data_801c4c80_slot04_12[obj->field_07](obj);
}

void func_801b32a0_slot04_12(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        func_80130678(obj, 0x2f);
    } else {
        func_801204f4(obj, obj->side, 8);
        func_80130678(obj, 0x2b);
    }
}

void func_801b3324_slot04_12(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = -3;
            b = 0x50;
        } else {
            a = -7;
            b = 0x47;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b353c_slot04_12(obj);
    }
    func_80130efc(obj);
}

void func_801b33c8_slot04_12(Object *obj) {
    int a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_46 = (u8)obj->field_46 + 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        } else {
            a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b345c_slot04_12(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    func_80130efc(o);
    o->field_46 = t = o->field_46 - 1;
    if ((t & 0xff) != 0) {
        func_801b34f0_slot04_12(o);
    } else {
        if (o->field_45 != 0) {
            o->field_04 = 1;
        }
        o->field_06 = 3;
        o->field_05 = 0;
        o->field_07 = 1;
        func_80142c70(o);
    }
}

void func_801b34f0_slot04_12(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = obj->field_27c;
        }
    }
    func_80142fe8(o);
}

void func_801b353c_slot04_12(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x50; i >= 0; i--) {
        *p++ = z;
    }
}

void func_801b3560_slot04_12(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b3748_slot04_12(obj);
    } else {
        func_801b35a0_slot04_12(obj);
    }
}

void func_801b35a0_slot04_12(Object *obj) {
    data_801c4c90_slot04_12[obj->field_07](obj);
}

void func_801b35e0_slot04_12(Object *obj) {
    obj->field_07++;
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_80141f28(obj, 3);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (!((s16)(box_margin + 0xc0) < obj->pos_x)) {
            obj->field_0b = 1;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x18);
}

void func_801b3690_slot04_12(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_801204f4(obj, obj->side, 0xc);
        func_80140770(obj, 4, 0xa, 0xf, 0, 0, 1);
    }
    func_80130efc(obj);
}

void func_801b3704_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3748_slot04_12(Object *obj) {
    data_801c4c9c_slot04_12[obj->field_07](obj);
}

void func_801b3788_slot04_12(Object *obj) {
    obj->field_07++;
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_80141f28(obj, 4);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (!((s16)(box_margin + 0xc0) < obj->pos_x)) {
            obj->field_0b = 1;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    obj->field_46 = (u8)obj->field_46 + 0xc00;
    func_801307e0(obj, 0x19);
}

void func_801b3844_slot04_12(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
    }
}

void func_801b3878_slot04_12(Object *obj) {
    if (func_801b3a4c_slot04_12(obj) < 0 && obj->pos_y >= obj->field_70) {
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 2;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801307e0(obj, 0x1a);
    } else {
        if (((Slot04bObj *)obj)->field_3a != 0) {
            obj->field_07 = obj->field_07 + 1;
            func_801204f4(obj, obj->side, 0xc);
            func_80140770(obj, 4, 0xa, 0x12, 0, 0, 1);
        }
        func_80130efc(obj);
    }
}

void func_801b3940_slot04_12(Object *obj) {
    if (func_801b3a4c_slot04_12(obj) < 0 && obj->pos_y > obj->field_70) {
        func_80131638(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b39a0_slot04_12(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_801204f4(obj, obj->side, 0xc);
    func_80140770(obj, 4, 0xa, 0xf, 0, 0, 1);
    func_801b3a08_slot04_12(obj);
}

void func_801b3a08_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

int func_801b3a4c_slot04_12(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    return obj->field_50 = obj->field_50 + obj->field_58;
}
