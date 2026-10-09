/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7930_slot04_14[];
extern u8 data_801c7938_slot04_14[];
extern ObjectFn data_801c7948_slot04_14[];
extern u8 data_801c7950_slot04_14[];
extern s32 data_801c7960_slot04_14[];
extern ObjectFn data_801c799c_slot04_14[];
extern ObjectFn data_801c79a4_slot04_14[];
extern ObjectFn data_801c7f34_slot04_14[];
extern void *data_801c7864_slot04_14[];
extern void *data_801c784c_slot04_14[];
extern void *data_801c7858_slot04_14[];
extern void *data_801c7838_slot04_14[];
extern void *data_801c7804_slot04_14[];
extern void *data_801c7818_slot04_14[];
extern void *data_801c77dc_slot04_14[];
extern void *data_801c7744_slot04_14[];
extern void *data_801c7784_slot04_14[];
extern void *data_801c7704_slot04_14[];
extern void *data_801c766c_slot04_14[];
extern void *data_801c76ac_slot04_14[];
extern SequenceStep *data_801c5af4_slot04_14[];

int func_801b7d28_slot04_14(Object *obj, Object *parent);
void func_801b7da8_slot04_14(Object *obj, u8 a);
void func_801b7df8_slot04_14(Object *obj, u8 a);
void func_801b7e48_slot04_14(Object *obj, u8 a);
void func_801b7e98_slot04_14(Object *obj, u8 a);
void func_801b7ee8_slot04_14(Object *obj, void **tbl);
void func_801b7f04_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b7f1c_slot04_14(Object *obj, u8 a);
void func_801b8048_slot04_14(Object *obj);
void func_801b810c_slot04_14(Object *obj);
void func_801b8140_slot04_14(Object *obj);
void func_801b8170_slot04_14(Object *obj);
void func_8011f14c(Slab172 *o);
void func_80137be0(Object *obj);
void func_80137cc0(Object *obj);
void func_8011ffdc(Object *obj);

void func_801b781c_slot04_14(Object *obj) {
    if (obj->pos_y >= (s16)(obj->field_70 - 0x10)) {
        func_801b7f04_slot04_14(obj, 2, 0, 0, 0);
        obj->pos_y = obj->field_70;
        func_801b7f1c_slot04_14(obj, 0x10);
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    }
}

void func_801b78ac_slot04_14(Object *obj) {
    if (obj->field_03 != 0) {
        func_801b7d28_slot04_14(obj, obj->field_3c);
    }
    func_80137be0(obj);
}

void func_801b78f4_slot04_14(Object *obj) {
    func_80137cc0(obj);
}

void func_801b7914_slot04_14(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7930_slot04_14[obj->field_06](obj);
    }
    func_8011ffdc(obj);
}

void func_801b7980_slot04_14(Object *obj) {
    u8 a;

    obj->field_46 = 2;
    obj->field_06++;
    obj->field_0b ^= 1;
    func_801380f0(obj);
    a = data_801c7938_slot04_14[obj->field_ac];
    if (a != 0) {
        func_801b7f1c_slot04_14(obj, a);
    }
    func_80131094(obj);
}

void func_801b79fc_slot04_14(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_00 = 1;
        func_801b7f04_slot04_14(obj, 1, 0, 0, 0);
        obj->field_4c = -obj->field_4c;
        obj->field_50 = 0;
    }
    func_80131094(obj);
}

void func_801b7a6c_slot04_14(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7948_slot04_14[obj->field_06](obj);
    }
    func_8011ffdc(obj);
}

void func_801b7ad8_slot04_14(Object *obj) {
    u8 a;

    obj->field_46 = 2;
    obj->field_06++;
    obj->field_0b ^= 1;
    func_801380f0(obj);
    a = data_801c7950_slot04_14[obj->field_ac];
    if (a == 0x1d || a == 0x1e || a == 0x1f) {
        obj->field_0d++;
    }
    if (a != 0) {
        func_801b7f1c_slot04_14(obj, a);
    }
}

void func_801b7b7c_slot04_14(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_00 = 1;
        func_801b7f04_slot04_14(obj, 1, 0, 0, 0);
        obj->field_4c = -obj->field_4c;
        obj->field_50 = data_801c7960_slot04_14[obj->field_ac];
    }
    func_80131094(obj);
}

void func_801b7c08_slot04_14(Object *obj) {
    data_801c799c_slot04_14[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801b7c5c_slot04_14(Object *obj) {
    int a = 0x10;

    obj->field_05++;
    if (obj->field_03 != 2 && obj->field_03 != 8) {
        a = 0xf;
    }
    func_801b7f1c_slot04_14(obj, a);
}

void func_801b7ca8_slot04_14(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b7f04_slot04_14(obj, 3, 0, 0, 0);
    }
    func_80131094(obj);
}

void func_801b7cf4_slot04_14(Object *obj) {
    Object *p = obj->field_3c;
    u8 n = p->field_240 - 1;

    p->field_14c = 0;
    p->field_240 = n;
    func_8011f14c((Slab172 *)obj);
}

int func_801b7d28_slot04_14(Object *obj, Object *parent) {
    u8 a = obj->field_ac;

    if (obj->field_03 == 6) {
        func_801b7e98_slot04_14(obj, a);
        return 1;
    }
    if (obj->field_03 == 8) {
        func_801b7e48_slot04_14(obj, a);
        return 1;
    }
    if (obj->field_03 == 4) {
        func_801b7df8_slot04_14(obj, a);
        return 1;
    }
    if (obj->field_03 == 2) {
        func_801b7da8_slot04_14(obj, a);
        return 1;
    }
    return -1;
}

void func_801b7da8_slot04_14(Object *obj, u8 a) {
    void **t = data_801c7864_slot04_14;

    if (a == 3) {
        t = data_801c784c_slot04_14;
    }
    if (a == 4) {
        t = data_801c7858_slot04_14;
    }
    func_801b7ee8_slot04_14(obj, t);
}

void func_801b7df8_slot04_14(Object *obj, u8 a) {
    void **t = data_801c7838_slot04_14;

    if (a == 6) {
        t = data_801c7804_slot04_14;
    }
    if (a == 7) {
        t = data_801c7818_slot04_14;
    }
    func_801b7ee8_slot04_14(obj, t);
}

void func_801b7e48_slot04_14(Object *obj, u8 a) {
    void **t = data_801c77dc_slot04_14;

    if (a == 0xc) {
        t = data_801c7744_slot04_14;
    }
    if (a == 0xd) {
        t = data_801c7784_slot04_14;
    }
    func_801b7ee8_slot04_14(obj, t);
}

void func_801b7e98_slot04_14(Object *obj, u8 a) {
    void **t = data_801c7704_slot04_14;

    if (a == 9) {
        t = data_801c766c_slot04_14;
    }
    if (a == 0xa) {
        t = data_801c76ac_slot04_14;
    }
    func_801b7ee8_slot04_14(obj, t);
}

void func_801b7ee8_slot04_14(Object *o, void **tbl) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_6c = tbl[obj->field_5c];
}

void func_801b7f04_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->field_04 = a;
    obj->field_05 = b;
    obj->field_06 = c;
    obj->field_07 = d;
}

void func_801b7f1c_slot04_14(Object *obj, u8 a) {
    if (obj->field_66 != 0) {
        func_80130700(obj, seqs_154_right[a]);
    } else {
        func_80130700(obj, seqs_a4_left[a]);
    }
}

void func_801b7f70_slot04_14(Object *obj) {
    data_801c79a4_slot04_14[obj->field_04](obj);
}

void func_801b7fb0_slot04_14(Object *obj) {
    Object *p = obj->field_3c;

    obj->field_04++;
    obj->field_1c = p->field_1c;
    obj->field_03 = p->kind;
    obj->field_0c = p->field_0c;
    obj->field_0d = p->field_0d;
    obj->field_0e = p->field_0e;
    obj->field_7a = 0x60;
    obj->field_48 = 0;
    obj->field_7c = 0x1e0;
    obj->field_90 = p->field_90;
    obj->field_98 = p->field_98;
    obj->field_9c = p->field_9c;
    func_801b8048_slot04_14(obj);
}

void func_801b8048_slot04_14(Object *obj) {
    Object *p = obj->field_3c;
    u16 t;
    u16 u;
    u16 w;

    obj->field_01 = 0;
    if (obj->field_03 != p->kind) {
        func_801b810c_slot04_14(obj);
    } else {
        w = p->field_3a;
        t = w & 0xff;
        u = t;
        if (u == 0) {
            obj->field_48 = 0;
        } else if ((w & 0x80) != 0) {
            func_801b810c_slot04_14(obj);
        } else {
            func_801b8170_slot04_14(obj);
            obj->field_01 = 1;
            if (obj->field_48 != u) {
                obj->field_48 = t;
                func_801b8140_slot04_14(obj);
            } else {
                func_80131094(obj);
                func_8011ffdc(obj);
            }
        }
    }
}

void func_801b810c_slot04_14(Object *obj) {
    obj->field_04++;
}

void func_801b8120_slot04_14(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b8140_slot04_14(Object *obj) {
    func_80130768(obj, (u8)(obj->field_48 - 1), data_801c5af4_slot04_14);
}

void func_801b8170_slot04_14(Object *obj) {
    Object *p = obj->field_3c;

    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0b = 0;
    if (p->field_0b != 0) {
        obj->pos_x = obj->pos_x - 0x18;
    }
}

void func_801b81b8_slot04_14(Object *obj) {
    data_801c7f34_slot04_14[obj->field_04](obj);
}
