/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c64c0_slot04_14[];
extern u8 data_801c64cc_slot04_14[];
extern u8 data_801c64d4_slot04_14[];
extern ObjectFn data_801c64dc_slot04_14[];

void func_80146960(Object *object);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
Block172 *func_8011f1e0(void);
void func_80130678(Object *object, int index);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801b4d4c_slot04_14(Object *object);
void func_801b61c4_slot04_14(Object *object);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b5a90_slot04_14(Object *object);
void func_801b5c34_slot04_14(Object *object);

void func_801b509c_slot04_14(Object *obj) {
    func_80130efc(obj);
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y >= obj->field_70) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_49 != 0) {
            obj->field_17b = 0;
            obj->field_45 = 0;
        }
        obj->field_4c = 0x20000;
        obj->pos_y = obj->field_70;
        obj->field_54 = data_801c64c0_slot04_14[obj->field_12a >> 1];
        func_801307e0(obj, 0x55);
        func_80120554(obj, obj->side, 0x324);
    }
}

void func_801b5190_slot04_14(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_07 = obj->field_07 + 1;
        func_801307e0(obj, 0x56);
    } else {
        func_80130efc(obj);
        func_801b4d4c_slot04_14(obj);
    }
}

void func_801b5230_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_157 = 0;
        obj->field_45 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5278_slot04_14(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_0b = obj->field_0b ^ 1;
        obj->pos_y = obj->pos_y + 0x27;
        if (obj->field_0b != 0) {
            obj->pos_x = obj->pos_x - 0x11;
        } else {
            obj->pos_x = obj->pos_x + 0x11;
        }
    }
}

void func_801b5304_slot04_14(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_50 = 0;
        obj->field_54 = 0;
        obj->field_4c = 0;
        obj->field_58 = -0x9000;
        func_801307e0(obj, 0x58);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5394_slot04_14(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0 && obj->pos_y >= obj->field_70) {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_80146960(obj);
        game_state.field_63 = data_801c64cc_slot04_14[obj->field_12a & 0xfe];
        func_80120554(obj, obj->side, 0x319);
        func_801307e0(obj, 0x59);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5478_slot04_14(Object *obj) {
    Object *p;
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        p = obj->other;
        p->field_15b = 1;
        func_80140770(obj, 6, 0xf, *(s16 *)(data_801c64d4_slot04_14 + (obj->field_12a & 0xfe)), 4, 1, 0);
        obj->field_4c = 0x28000;
        obj->field_50 = 0x70000;
        obj->field_54 = 0;
        obj->field_58 = -0x5400;
        if ((s16)p->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x22;
            }
        }
    }
}

void func_801b5550_slot04_14(Object *obj) {
    if ((u8)obj->field_3a == 0) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
        obj->field_50 = obj->field_50 + obj->field_58;
        if (obj->pos_y >= obj->field_70) {
            obj->field_45 = 0;
            obj->pos_y = obj->field_70;
            func_801209c4(obj);
            func_801312b8(obj);
            return;
        }
    }
    func_80130efc(obj);
}

void func_801b5614_slot04_14(Object *obj) {
    u16 t;
    func_80130efc(obj);
    t = obj->field_3a;
    if ((u8)t != 0) {
        obj->field_3a = t & 0xff00;
        func_80120554(obj, obj->side, 0x320);
    }
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y >= obj->field_70) {
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x5d);
    }
}

void func_801b5708_slot04_14(Object *obj) {
    data_801c64dc_slot04_14[obj->field_07](obj);
}

void func_801b5748_slot04_14(Object *obj) {
    Object *p;
    int a;
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        a = 0x2f;
    } else {
        p = (Object *)func_8011f1e0();
        a = 0x2b;
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x7d;
            p->field_3c = obj;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_0e = obj->field_0e;
            obj->field_3c = p;
            p->field_02 = 0x12;
            p->field_08 = 0x20;
            p->field_66 = obj->field_66;
        }
    }
    func_80130678(obj, a);
}

void func_801b581c_slot04_14(Object *obj) {
    int a;
    int b;
    if ((s16)obj->field_3a != 0) {
        a = 1;
        b = 0x47;
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = 0xd;
            b = 0x39;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b61c4_slot04_14(obj);
    }
    func_80130efc(obj);
}

void func_801b58b0_slot04_14(Object *obj) {
    int a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        a = ((Slot04bObj *)obj)->field_c6;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_27c = 0;
        obj->field_46 = obj->field_46 | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b5938_slot04_14(Object *obj) {
    Object *p;
    s16 t;
    u16 u;
    obj->field_27c = obj->field_27c + 1;
    func_80130efc(obj);
    u = obj->field_46 - 1;
    obj->field_46 = u;
    if ((u8)u == 0) {
        if (obj->field_45 != 0) {
            func_801b62a8_slot04_14(obj, 1, 0, 3, 1);
        } else {
            p = obj->field_3c;
            if (p->field_02 == 0x12) {
                p->field_04 = 2;
                p->field_07 = 0;
                p->field_06 = 0;
                p->field_05 = 0;
            }
        }
        func_80142c70(obj);
    }
    if ((obj->field_134 & 0xff00) != 0) {
        if (((Slot04bObj *)obj)->field_27d == 0) {
            ((Slot04bObj *)obj)->field_27d = obj->field_27c;
        }
    } else {
        t = obj->field_46;
        if ((t & 0xff00) != 0) {
            obj->field_46 = t - 0x100;
        }
        func_80142fe8(obj);
    }
}

void func_801b5a40_slot04_14(Object *obj) {
    if (obj->field_129 == 0 || obj->field_12a != 4) {
        func_801b5a90_slot04_14(obj);
    } else {
        func_801b5c34_slot04_14(obj);
    }
}
