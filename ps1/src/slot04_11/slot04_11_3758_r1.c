/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7568_slot04_11[];
extern ObjectFn data_801c75d0_slot04_11[];
extern s32 data_801c7590_slot04_11[];
extern s16 data_801c75c0_slot04_11[];
extern u8 data_801c75fc_slot04_11[];
extern u8 data_801c75fd_slot04_11[];
extern s32 data_801c7604_slot04_11[];
extern ObjectRef data_80190468;

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_80146960(Object *object);
void func_80141e5c(Object *object);
int func_8013fd98(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801483a4(Object *object, int a, int b);
int func_80140cd8(Object *object, int a, int b);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b5874_slot04_11(Object *obj);
int func_801b57a8_slot04_11(Object *obj);
int func_801b5764_slot04_11(Object *obj);

void func_801b3758_slot04_11(Object *obj) {
    int a = obj->field_4c;

    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
    obj->field_4c = obj->field_4c + obj->field_54;
    if ((s32)obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b37f8_slot04_11(Object *obj) {
    obj->field_167 = (obj->field_12a >> 1) + 0xc;
    if (obj->field_12f == 0) {
        obj->field_12f = 0xff;
        data_80190468.p->field_6b = 5;
        func_80147000(obj);
    }
}

void func_801b3844_slot04_11(Object *obj) {
    data_801c7568_slot04_11[obj->field_07](obj);
}

void func_801b3884_slot04_11(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x40);
}

void func_801b38e0_slot04_11(Object *obj) {
    int a;

    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        a = -1;
        obj->field_07++;
        obj->field_0b = obj->field_158;
        obj->field_10 = 0;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_801483a4(obj, 0xc, 0x4d);
        func_80120554(obj, obj->side, 0x31c);
        func_801204f4(obj, obj->side, 0xa);
    }
}

void func_801b3970_slot04_11(Object *obj) {
    int i;

    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        obj->field_07++;
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
        }
        i = obj->field_12a;
        obj->field_27b = 0;
        obj->field_165 = 0;
        obj->field_4c = data_801c7590_slot04_11[i * 2];
        obj->field_54 = data_801c7590_slot04_11[i * 2 + 1];
        obj->field_50 = data_801c7590_slot04_11[i * 2 + 2];
        obj->field_58 = data_801c7590_slot04_11[i * 2 + 3];
        obj->field_45 = 1;
    }
}

void func_801b3a50_slot04_11(Object *obj) {
    if (func_801b57a8_slot04_11(obj) < 0) {
        obj->field_07 = 6;
        obj->field_48 = 0xff;
        func_801307e0(obj, 0x41);
    } else {
        ref_other.p = obj->other;
        if (ref_other.p->field_163 == 0) {
            if ((u8)func_8013fd98(obj, -0x20, 0x20, 0x1a, 0x1a) != 0) {
                obj->field_07++;
                func_80120554(obj, obj->other->side, 0x31a);
                func_801204f4(obj, obj->side, 0xd);
                func_801307e0(obj, obj->field_12a + 0x43);
            }
        }
        func_80130efc(obj);
    }
}

void func_801b3b34_slot04_11(Object *obj) {
    if ((u8)obj->field_3a == 0 && func_801b57a8_slot04_11(obj) < 0) {
        obj->field_58 = -0x3800;
        if (obj->field_70 - 8 <= obj->pos_y) {
            obj->field_07++;
            obj->field_14 = 0;
            obj->pos_y = (u16)((Slot04bObj *)obj)->field_70 - 8;
            obj->field_0b = obj->field_0b ^ 1;
            func_801307e0(obj, obj->field_12a + 0x44);
            return;
        }
    }
    func_80130efc(obj);
}

void func_801b3bd8_slot04_11(Object *obj) {
    int k = obj->field_3a & 0x7f;
    int j;
    int a;

    if (k != 0) {
        data_80190468.p->field_63 = 0x20;
        obj->field_3a = obj->field_3a & 0xff80;
        func_80146960(obj);
        func_801204f4(obj, obj->side, 8);
        j = k - 1;
        if (func_80140cd8(obj, data_801c75c0_slot04_11[j * 2], data_801c75c0_slot04_11[j * 2 + 1]) != 0) {
            func_80120554(obj, obj->side, 0x319);
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        } else {
            func_80120554(obj, obj->side, 0x319);
        }
    }
    if (obj->field_3a & 0x80) {
        obj->field_07++;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        func_80140770(obj, 0, 0xa, -0x200, 0, 0, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
        }
        a = 0x40000;
        if (obj->field_0b != 0) {
            a = -0x40000;
        }
        obj->field_50 = 0x60000;
        obj->field_4c = a;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
        obj->field_48 = 0;
    } else {
        func_80130efc(obj);
        func_80141e5c(obj);
    }
}

void func_801b3d94_slot04_11(Object *obj) {
    int r;

    if (obj->field_48 == 0) {
        r = func_801b5764_slot04_11(obj);
    } else {
        r = func_801b57a8_slot04_11(obj);
    }
    if (r < 0 && obj->pos_y >= obj->field_70) {
        obj->field_07++;
        func_801209c4(obj);
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801307e0(obj, 0x42);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3e40_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3ea0_slot04_11(Object *obj) {
    data_801c75d0_slot04_11[obj->field_07](obj);
}

void func_801b3ee0_slot04_11(Object *obj) {
    obj->field_07++;
    obj->field_225 = 1;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_49 == 0) {
        func_80145d20(obj);
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x49);
}

void func_801b3f58_slot04_11(Object *obj) {
    int a;

    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        a = -1;
        obj->field_07++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_801483a4(obj, -0x35, 0x37);
        func_80120554(obj, obj->side, 0x31c);
    }
}

void func_801b3fcc_slot04_11(Object *obj) {
    int i;
    int a;
    int b;

    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        i = 6;
        obj->field_07++;
        obj->field_12e = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            i = obj->field_12a;
        }
        obj->field_12f = data_801c75fc_slot04_11[i];
        obj->field_27b = data_801c75fd_slot04_11[i];
        a = data_801c7604_slot04_11[i * 2];
        b = data_801c7604_slot04_11[i * 2 + 1];
        obj->field_50 = data_801c7604_slot04_11[i * 2 + 2];
        obj->field_58 = data_801c7604_slot04_11[i * 2 + 3];
        if (obj->field_0b == 0) {
            a = -a;
            b = -b;
        }
        obj->field_4c = a;
        obj->field_54 = b;
        *(u16 *)&((Slot04bObj *)obj)->field_1c0 = 0;
    }
}
