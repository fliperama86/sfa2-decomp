/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ebc1c_slot06_0c[];
extern void (*data_801eaba4_slot06_0c[])(Object *, u16);
extern u16 data_801eabb8_slot06_0c[];
extern ObjectFn data_801eabc4_slot06_0c[];
extern u16 data_801eabd4_slot06_0c[];
extern u16 data_801eabd6_slot06_0c[];
extern u8 data_801eabdc_slot06_0c[];
extern ObjectFn data_801eabe0_slot06_0c[];

void func_801ea18c_slot06_0c(Object *obj);
void func_801ea1e0_slot06_0c(Object *obj);
void func_801ea43c_slot06_0c(Object *obj);
void func_801ea490_slot06_0c(Object *obj);

void func_801e9d74_slot06_0c(Object *obj, u16 unused) {
    int i;

    if (((s16)obj->field_3a & 0xff00) == 0) {
        func_80131094(obj);
        return;
    }
    if (obj->field_4c == 0x10) {
        obj->field_06 = 0;
        obj->field_05 = 0;
        func_80131094(obj);
        return;
    }
    if (obj->field_4c == 0x11) {
        obj->field_05 = 1;
        obj->field_06 = 2;
        i = 0xc;
    } else if (obj->field_4c == 0x12) {
        obj->field_06 = 2;
        obj->field_05 = 2;
        obj->field_0b = 1;
        i = 0xc;
    } else if (obj->field_4c == 0x13) {
        obj->field_05 = 1;
        obj->field_06 = 0;
        i = 0xb;
    } else {
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_0b = 1;
        i = 0xb;
    }
    func_80130700(obj, data_801ebc1c_slot06_0c[i]);
}

void func_801e9e5c_slot06_0c(Object *obj, int arg) {
    data_801eaba4_slot06_0c[obj->field_06](obj, arg);
}

void func_801e9e9c_slot06_0c(Object *obj, u16 arg) {
    int i;

    if (arg == 1) {
        func_801ea18c_slot06_0c(obj);
        func_801ea1e0_slot06_0c(obj);
        func_80131094(obj);
        return;
    }
    i = 5;
    if (arg != 2) {
        obj->field_06 = 0;
        obj->field_05 = 0;
    } else {
        obj->field_4c = 0;
        obj->field_06++;
        i = 0xc;
    }
    func_80130700(obj, data_801ebc1c_slot06_0c[i]);
}

void func_801e9f34_slot06_0c(Object *obj, u16 unused) {
    int i = 0xb;
    int v;

    if (((s16)obj->field_3a & 0xff00) != 0) {
        v = 0;
        if (obj->field_4c == 0) {
            i = 0xd;
            v = 2;
        }
        obj->field_06 = v;
        func_80130700(obj, data_801ebc1c_slot06_0c[i]);
    } else {
        func_80131094(obj);
    }
}

void func_801e9fa8_slot06_0c(Object *obj, u16 arg) {
    int i;

    if (arg == 2) {
        func_80131094(obj);
        return;
    }
    if (arg == 1) {
        obj->field_06 = 1;
        obj->field_4c = arg;
        i = 0xc;
    } else {
        obj->field_06 = 0;
        obj->field_05 = 0;
        i = 5;
    }
    func_80130700(obj, data_801ebc1c_slot06_0c[i]);
}

void func_801ea020_slot06_0c(Object *obj, u16 arg) {
    if (((s16)obj->field_3a & 0xff00) != 0) {
        obj->field_06++;
        obj->field_4c = data_801eabb8_slot06_0c[arg];
        func_80130700(obj, data_801ebc1c_slot06_0c[data_801eabb8_slot06_0c[arg]]);
    } else {
        func_80131094(obj);
    }
}

void func_801ea0ac_slot06_0c(Object *obj, u16 unused) {
    int i;

    if (((s16)obj->field_3a & 0xff00) == 0) {
        func_80131094(obj);
        return;
    }
    if (obj->field_4c == 0x10) {
        obj->field_06 = 0;
        obj->field_05 = 0;
        func_80131094(obj);
        return;
    }
    if (obj->field_4c == 0x11) {
        obj->field_06 = 2;
        obj->field_05 = 2;
        i = 0xc;
    } else if (obj->field_4c == 0x12) {
        obj->field_05 = 1;
        obj->field_06 = 2;
        obj->field_0b = 0;
        i = 0xc;
    } else if (obj->field_4c == 0x13) {
        obj->field_05 = 2;
        obj->field_06 = 0;
        i = 0xb;
    } else {
        obj->field_05 = 1;
        obj->field_0b = 0;
        obj->field_06 = 0;
        i = 0xb;
    }
    func_80130700(obj, data_801ebc1c_slot06_0c[i]);
}

void func_801ea18c_slot06_0c(Object *obj) {
    if (player_left.field_06 == 7 || player_right.field_06 == 7) {
        obj->field_06 = 3;
        func_80130700(obj, data_801ebc1c_slot06_0c[14]);
    }
}

void func_801ea1e0_slot06_0c(Object *obj) {
    if (player_left.field_06 == 8 || player_right.field_06 == 8) {
        obj->field_06 = 3;
        func_80130700(obj, data_801ebc1c_slot06_0c[15]);
    }
}

int func_801ea234_slot06_0c(Object *obj) {
    int r;
    s16 rt;
    s16 l;
    u16 x;
    s16 lo;
    s16 hi;
    s16 d;

    rt = player_right.pos_x;
    l = player_left.pos_x;
    x = obj->pos_x;
    if (l <= rt) {
        lo = l;
        hi = rt;
    } else {
        lo = rt;
        hi = l;
    }
    d = hi - x;
    if (d >= 0) {
        d = lo - x;
        r = 0;
        if (d >= 0) {
            if (d < 0xc0) {
                r = 1;
            } else {
                r = 2;
            }
        }
    } else {
        if (d >= -0xbf) {
            r = 3;
        } else {
            r = 4;
        }
    }
    return r;
}

void func_801ea2c8_slot06_0c(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801ea2e8_slot06_0c(Object *obj) {
    data_801eabc4_slot06_0c[obj->field_04](obj);
}

void func_801ea328_slot06_0c(Object *obj) {
    obj->field_04 = 1;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_50 = 0x1c0;
    obj->field_48 = 2;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_81 = 4;
    obj->field_50 -= 0x20;
    func_801ea490_slot06_0c(obj);
    func_801ea43c_slot06_0c(obj);
}

void func_801ea394_slot06_0c(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->field_50 >= (s16)((Slot06Layer *)data_801aa5d4)->field_12) {
            if (obj->field_48 != 0) {
                obj->field_48 = 0;
                func_801ea490_slot06_0c(obj);
                func_801ea43c_slot06_0c(obj);
            }
        } else if (obj->field_48 == 0) {
            obj->field_48 = 2;
            func_801ea490_slot06_0c(obj);
            func_801ea43c_slot06_0c(obj);
        }
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801ea43c_slot06_0c(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    obj->pos_x = data_801eabd4_slot06_0c[obj->field_48];
    obj->pos_y = 0x100 - data_801eabd6_slot06_0c[obj->field_48];
}

void func_801ea490_slot06_0c(Object *obj) {
    func_80130700(obj, data_801ebc1c_slot06_0c[data_801eabdc_slot06_0c[obj->field_48 >= 2]]);
}

void func_801ea4e8_slot06_0c(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801ea508_slot06_0c(Object *obj) {
    data_801eabe0_slot06_0c[obj->field_04](obj);
}

void func_801ea548_slot06_0c(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_81 = 4;
    obj->field_4c = *(s32 *)&obj->field_10;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_04++;
    obj->field_54 = 0xf80000 - *(s32 *)&obj->field_14;
    func_80130700(obj, data_801ebc1c_slot06_0c[0]);
}

void func_801ea5b0_slot06_0c(Object *obj) {
    Slot06Layer *l = (Slot06Layer *)data_801aa5d4;
    s32 a;

    a = *(s32 *)&l->field_08 - *(s32 *)&l->field_20;
    a >>= 1;
    a += obj->field_4c;
    *(s32 *)&obj->field_10 = a;
    a = *(s32 *)&l->field_0c - *(s32 *)&l->field_24;
    a >>= 1;
    a += obj->field_54;
    *(s32 *)&obj->field_14 = 0x1000000 - a;
    func_80120028(obj);
}

void func_801ea620_slot06_0c(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
