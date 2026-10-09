/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5be4_slot04_0f[];
extern ObjectFn data_801c5bf0_slot04_0f[];
extern s8 data_801c5c0c_slot04_0f[];
extern u8 data_801c5c1c_slot04_0f[];

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80130678(Object *object, int arg);
void func_80131468(Object *object);
int func_80141618(Object *object);
int func_801b4ad8_slot04_0f(Object *obj);
u16 *func_801b4c64_slot04_0f(Object *obj);
int func_801b4cac_slot04_0f(Object *obj, u16 *mask, BytePair dir);
void func_80138c78(GameState *state, Object *object);
void func_801b4908_slot04_0f(Object *obj);

void func_801b44e0_slot04_0f(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            if (obj->field_130 & 0x1000) {
                func_80130280(obj);
            }
        }
    }
}

void func_801b4568_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b45a8_slot04_0f(Object *obj) {
    data_801c5be4_slot04_0f[obj->field_07](obj);
}

void func_801b45e8_slot04_0f(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->other->side, 0x31a);
    obj->field_0b = 0;
    if ((obj->field_c2 & 0x2000) == 0) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x19);
}

void func_801b465c_slot04_0f(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0x12, 0, 1, 0);
    }
    func_80130efc(obj);
}

void func_801b46c0_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b470c_slot04_0f(Object *obj) {
    func_801312b8(obj);
}

void func_801b472c_slot04_0f(Object *obj) {
    func_80131468(obj);
}

void func_801b474c_slot04_0f(Object *obj) {
    data_801c5bf0_slot04_0f[obj->field_06](obj);
}

void func_801b478c_slot04_0f(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b47c0_slot04_0f(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b4818_slot04_0f(Object *obj) {
    Config *c;
    int r;

    obj->field_06 = obj->field_06 + 1;
    obj->field_46 = 0x3c;
    c = game_state.config;
    if (c->field_76 == 0) {
        c->field_76 = 0x1e;
    }
    r = func_80125734(obj, data_801c5c0c_slot04_0f[func_80151184() & 0xf]);
    obj->field_06 = data_801c5c1c_slot04_0f[(u8)r];
    func_80130678(obj, (r & 0xff) + 0x23);
}

void func_801b48bc_slot04_0f(Object *obj) {
    if (*(u8 *)&obj->field_3a == 1) {
        obj->field_50 = 0x18000;
        obj->field_58 = -0x2000;
        obj->field_06 = obj->field_06 + 1;
    }
    func_801b4908_slot04_0f(obj);
}

void func_801b4908_slot04_0f(Object *obj) {
    s16 t = obj->field_46;
    if (t != 0) {
        t = t - 1;
        obj->field_46 = t;
        if (t == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    func_80130efc(obj);
}

void func_801b496c_slot04_0f(Object *obj) {
    *(u32 *)&obj->field_14 = *(u32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 >= 0) {
        obj->field_50 = 0x18000;
        obj->field_58 = -0x2000;
        obj->field_06 = obj->field_06 + 1;
    }
    func_801b4908_slot04_0f(obj);
}

void func_801b49d4_slot04_0f(Object *obj) {
    *(u32 *)&obj->field_14 = *(u32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0) {
        obj->field_50 = -0x18000;
        obj->field_58 = 0x2000;
        obj->field_06--;
    }
    func_801b4908_slot04_0f(obj);
}

void func_801b4a3c_slot04_0f(Object *obj) {
    int t = (s16)obj->field_3a;

    if (t >= 0) {
        if ((s16)t < 0 && func_80141618(obj) != 0 || func_801b4ad8_slot04_0f(obj) != 0) {
            obj->field_07 = 0;
            obj->field_25f = 0xff;
        }
        func_80130efc(obj);
    } else {
        if (obj->field_128 == 0) {
            func_801312b8(obj);
        } else {
            func_80131468(obj);
        }
    }
}

/* The call of func_80138c78 passes two arguments although the callee takes none: the original sets the first two argument registers before it. Written without the arguments, this function differs from the original in 9 instruction slots. */
int func_801b4ad8_slot04_0f(Object *obj) {
    BytePair dir;
    u16 *a;
    if ((game_state.config->field_4d | game_state.config->field_04) != 0) {
        return 0;
    }
    if ((func_8012f56c(obj) & 0xff) != 0 || obj->field_67 == 0 || (obj->field_134 & 0xfc) == 0) {
        return 0;
    }
    a = func_801b4c64_slot04_0f(obj);
    dir = func_8013054c(obj);
    if (obj->field_7e == 0) {
        if (func_801b4cac_slot04_0f(obj, a, dir) == 0) return 0;
    }
    dir.first = obj->field_12a;
    dir.second = obj->field_129;
    obj->field_128 = 0;
    if (obj->field_130 & 0x4000) obj->field_128 = 2;
    func_8014147c(obj, dir);
    obj->field_17d = dir.first;
    obj->field_17c = dir.second;
    obj->field_17e = obj->field_128;
    ((void (*)(Config *, Object *))func_80138c78)(game_state.config, obj);
    func_80142c04(obj);
    return 1;
}
