/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c649c_slot04_14[];
extern u16 data_801c64a8_slot04_14[];
extern u16 data_801c64ac_slot04_14[];
extern u8 data_801c64b4_slot04_14[];
extern u8 data_801c64bc_slot04_14[];

int func_8014025c(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_80141e5c(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_801b4848_slot04_14(Object *obj);
void func_801b4968_slot04_14(Object *obj);
void func_801b4af4_slot04_14(Object *obj);
void func_801b4b88_slot04_14(Object *obj);
void func_801b4bc0_slot04_14(Object *obj);
void func_801b4c58_slot04_14(Object *obj);
void func_801b4d4c_slot04_14(Object *obj);

void func_801b4700_slot04_14(Object *obj) {
    u16 a;

    func_80130efc(obj);
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y >= obj->field_70) {
        obj->field_157 = 1;
        obj->field_159 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        a = obj->field_12a >> 1;
        obj->field_54 = data_801c649c_slot04_14[a];
        if (obj->field_49 != 0) {
            a += 0x6d;
        } else {
            a += 0x4d;
        }
        func_801307e0(obj, a);
        func_80120554(obj, obj->side, 0x324);
    } else if (obj->field_cd != 0) {
        func_801b4968_slot04_14(obj);
    } else {
        func_801b4848_slot04_14(obj);
    }
}

void func_801b4848_slot04_14(Object *obj) {
    if (obj->field_50 < 0) {
        if ((obj->field_134 | obj->field_136) & 0x95) {
            if (obj->other->field_164 == 0 && (u8)func_8014025c(obj, -8, 0x24, -0x54, 0x18)) {
                func_801b4af4_slot04_14(obj);
                return;
            }
            func_801b4b88_slot04_14(obj);
        }
        if (obj->field_50 < 0) {
            if ((obj->field_134 | obj->field_136) & 0x6a) {
                if (obj->other->field_164 == 0 && (u8)func_8014025c(obj, -8, 0x24, -0x30, 0x18)) {
                    func_801b4bc0_slot04_14(obj);
                } else {
                    func_801b4c58_slot04_14(obj);
                }
            }
        }
    }
}

void func_801b4968_slot04_14(Object *obj) {
    s16 d;

    if (obj->field_50 < 0 && obj->field_129 == 0) {
        if (obj->other->field_164 == 0) {
            d = obj->pos_x - obj->other->pos_x;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x19 && (u8)func_8014025c(obj, -8, 0x24, -0x54, 0x18)) {
                func_801b4af4_slot04_14(obj);
                return;
            }
            obj->field_46 = obj->field_46 - 0x100;
            func_801b4b88_slot04_14(obj);
        }
        if (obj->field_50 < 0 && obj->field_129 == 0 && obj->other->field_164 == 0) {
            d = obj->pos_x - obj->other->pos_x;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x19 && (u8)func_8014025c(obj, -8, 0x24, -0x30, 0x18)) {
                func_801b4bc0_slot04_14(obj);
            } else {
                obj->field_46 = obj->field_46 - 0x100;
                func_801b4c58_slot04_14(obj);
            }
        }
    }
}

void func_801b4af4_slot04_14(Object *obj) {
    Object *p;

    obj->field_07 = 6;
    obj->field_50 = 0x40000;
    obj->field_58 = -0x5800;
    obj->field_4c = -0x20000;
    p = obj->other;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x20000;
    }
    obj->field_54 = 0;
    func_80141f28(obj, 8);
    func_80120554(p, p->side, 0x31a);
    func_801307e0(obj, 0x4a);
    func_80141e5c(obj);
}

void func_801b4b88_slot04_14(Object *obj) {
    obj->field_07 = 0x11;
    obj->field_159 = 1;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x5a);
}

void func_801b4bc0_slot04_14(Object *obj) {
    Object *p;

    obj->field_50 = 0x58000;
    obj->field_58 = -0x1800;
    obj->field_4c = -0x20000;
    p = obj->other;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x20000;
    }
    obj->field_54 = 0;
    obj->field_07 = 0xc;
    func_80141f28(obj, 8);
    func_80120554(p, p->side, 0x31a);
    func_801307e0(obj, 0x57);
    func_80141e5c(obj);
}

void func_801b4c58_slot04_14(Object *obj) {
    obj->field_07 = 9;
    obj->field_159 = 1;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x52);
}

void func_801b4c90_slot04_14(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_17b = 0;
        obj->field_38 = 1;
        func_801307e0(obj, 0x50);
    } else {
        if ((u8)obj->field_3a == 0) {
            func_80130efc(obj);
        }
        func_801b4d4c_slot04_14(obj);
    }
}

void func_801b4d4c_slot04_14(Object *obj) {
    int i;

    if ((game_state.field_32 & 3) == 0) {
        i = 1;
        if ((u8)obj->field_3a != 0) {
            i = 2;
        }
        for (; i >= 0; i--) {
            u16 a;

            if (!func_80148e84(obj)) {
                break;
            }
            if ((u8)obj->field_3a != 0) {
                a = data_801c64ac_slot04_14[i];
            } else {
                a = data_801c64a8_slot04_14[i];
            }
            if (obj->field_0b != 0) {
                a = -a;
            }
            ref_other.p->pos_x = a + ref_other.p->pos_x;
        }
    }
}

void func_801b4e34_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4e78_slot04_14(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
        obj->field_50 = obj->field_50 + obj->field_58;
        if (obj->pos_y >= obj->field_70) {
            obj->field_07 = obj->field_07 + 1;
            obj->field_45 = 0;
            obj->pos_y = obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x4b);
        }
    }
}

void func_801b4f1c_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    func_80130efc(obj);
    p = obj->other;
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_17b = 0;
        p->field_15b = 1;
        func_80140770(obj, 0, 0xf, *(s16 *)(data_801c64b4_slot04_14 + (obj->field_12a & 0xfe)), 4, 0, 1);
        func_801307e0(obj, 0x51);
        if (((Slot04bObj *)p)->field_5c < 0) {
            if (obj->field_49 != 0) {
                obj->field_167 = 0x12;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            } else {
                obj->field_167 = 2;
            }
        }
    }
    t = obj->field_3a;
    if ((u8)t != 0) {
        obj->field_3a = t & 0xff00;
        func_80146478(obj, 2, -0x17, 0x26);
        func_80120554(p, p->side, 0x306);
        if (game_state.field_5c == 0) {
            game_state.field_5c = 0xa;
            game_state.field_5d = 2;
            game_state.field_5e = 2;
        }
        game_state.field_63 = data_801c64bc_slot04_14[obj->field_12a >> 1];
    }
}
