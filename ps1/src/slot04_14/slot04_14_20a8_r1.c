/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c62d4_slot04_14[];
extern s32 data_801c62e0_slot04_14[];
extern u16 data_801c62ec_slot04_14[];
extern ObjectFn data_801c62f4_slot04_14[];
extern s32 data_801c6304_slot04_14[];
extern ObjectFn data_801c6310_slot04_14[];

void func_80138ae8(GameState *state, Object *object);
Object *func_8011f0e8(void);
int func_80130184(Object *object);
void func_80130678(Object *object, u16 arg);
void func_801b2b4c_slot04_14(Object *obj);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
u8 func_801b26bc_slot04_14(Object *obj);
void func_801b2664_slot04_14(Object *obj);

void func_801b20a8_slot04_14(Object *obj) {
    int i = 1;

    obj->field_17b = i;
    obj->field_45 = i;
    obj->field_07 = i;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    i = obj->field_12a >> 1;
    *(s32 *)&obj->field_50 = 0x40000;
    obj->field_58 = -0x6000;
    *(u32 *)&obj->field_14 &= 0xffff0000;
    if (obj->field_49 != 0) {
        obj->field_58 = data_801c62d4_slot04_14[i];
    }
    obj->field_4c = -data_801c62e0_slot04_14[(i & 0xff)];
    if (obj->field_0b != 0) {
        obj->field_4c = data_801c62e0_slot04_14[(i & 0xff)];
    }
    func_801307e0(obj, data_801c62ec_slot04_14[(i & 0xff)]);
}

void func_801b2190_slot04_14(Object *obj) {
    u16 a;

    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    if (obj->field_50 < 0) {
        obj->field_07++;
        func_80120554(obj, obj->side, 0x320);
        if (obj->field_49 != 0) {
            a = 0x70;
        } else {
            a = 0x20;
        }
        ((Slot04bObj *)obj)->field_1a4 = 1;
        if (obj->field_12a != 0) {
            ((Slot04bObj *)obj)->field_1a4 = obj->field_12a;
        }
        a += obj->field_12a >> 1;
        func_801307e0(obj, a);
    } else {
        func_80130efc(obj);
    }
}

void func_801b225c_slot04_14(Object *obj) {

    if ((s16)obj->field_3a & 0x8000) {
        func_80120554(obj, obj->side, 0x320);
        ((Slot04bObj *)obj)->field_1a4--;
        if (((Slot04bObj *)obj)->field_1a4 == 0) {
            obj->field_07++;
            func_801307e0(obj, 0x1f);
            return;
        }
    }
    *(s32 *)&obj->field_10 += obj->field_4c;
    func_80130efc(obj);
}

void func_801b22f4_slot04_14(Object *obj) {
    Object *s1 = obj->other;

    if (func_801b26bc_slot04_14(obj) == 0) {
        func_801209c4(obj);
        s1->field_249 = 5;
        s1->field_14 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2364_slot04_14(Object *obj) {
    data_801c62f4_slot04_14[obj->field_07](obj);
}

void func_801b23a4_slot04_14(Object *obj) {
    s32 d;

    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    if (obj->field_50 < 0) {
        d = obj->field_58;
        obj->field_58 = d >> 1;
    } else {
        obj->field_58 = -0x4800;
    }
    d = data_801c6304_slot04_14[obj->field_12a >> 1];
    if (obj->field_0b != 0) {
        if (obj->field_4c < 0) {
            d = -d;
        }
    } else {
        if (obj->field_4c <= 0) {
            d = -d;
        }
    }
    obj->field_4c = d + obj->field_4c;
    func_80120554(obj, obj->side, 0x320);
    ((Slot04bObj *)obj)->field_1a4 = 1;
    if (obj->field_12a != 0) {
        ((Slot04bObj *)obj)->field_1a4 = obj->field_12a;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x23);
}

void func_801b24a8_slot04_14(Object *obj) {

    if (func_801b26bc_slot04_14(obj) != 0) {
        if ((s16)obj->field_3a & 0x8000) {
            func_80120554(obj, obj->side, 0x320);
            ((Slot04bObj *)obj)->field_1a4--;
            if (((Slot04bObj *)obj)->field_1a4 == 0) {
                obj->field_07++;
                func_801307e0(obj, 0x1f);
                return;
            }
        }
        func_80130efc(obj);
    } else {
        func_801b2664_slot04_14(obj);
    }
}

void func_801b2550_slot04_14(Object *obj) {
    if (func_801b26bc_slot04_14(obj) != 0) {
        if ((s16)obj->field_3a & 0x8000) {
            obj->field_07++;
            func_80130678(obj, 0x21);
        } else {
            func_80130efc(obj);
        }
    } else {
        func_801b2664_slot04_14(obj);
    }
}

void func_801b25cc_slot04_14(Object *obj) {
    Object *s1 = obj->other;

    if (func_801b26bc_slot04_14(obj) == 0) {
        func_801b62a8_slot04_14(obj, 1, 0, 3, 2);
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801209c4(obj);
        s1->field_249 = 5;
        func_80130678(obj, 0x11);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2664_slot04_14(Object *obj) {
    obj->field_07++;
    func_801209c4(obj);
    obj->field_159 = 0;
    obj->field_45 = 0;
    obj->field_14 = 0;
    obj->pos_y = ((Slot04bObj *)obj)->field_70;
    func_80130678(obj, 0x11);
}

u8 func_801b26bc_slot04_14(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    return obj->field_70 > obj->pos_y;
}

void func_801b26fc_slot04_14(Object *obj) {
    data_801c6310_slot04_14[obj->field_07](obj);
}

void func_801b273c_slot04_14(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80138ae8(&game_state, obj);
    func_801307e0(obj, 0x26);
}

void func_801b2790_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)func_80130184(obj) == 0) {
        func_801b2b4c_slot04_14(obj);
    } else {
        func_80130efc(obj);
        if ((u8)obj->field_3a == 1) {
            obj->field_07++;
            obj->field_3a = obj->field_3a & 0xff00;
            p = func_8011f0e8();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0x1a;
                p->field_ad = 0;
                p->field_5c = 0;
                p->field_03 = 0;
                p->field_66 = obj->field_66;
                p->field_65 = obj->field_65;
                p->field_ac = obj->field_12a >> 1;
                p->field_0b = obj->field_0b;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->field_0d = obj->field_0d;
                p->field_26 = obj->field_26;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = ((Slot04bObj *)obj)->field_70;
                p->field_7a = 0x60;
                p->field_3c = obj;
                p->field_7c = 0x1e0;
                ((Slot04bObj *)p)->field_70 = t;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                obj->field_14c = (s32)p;
                obj->field_240++;
                func_801204f4(obj, obj->side, 0xb);
            }
        }
    }
}

void func_801b2904_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)func_80130184(obj) == 0) {
        func_801b2b4c_slot04_14(obj);
    } else {
        func_80130efc(obj);
        if ((u8)obj->field_3a == 2) {
            obj->field_07++;
            obj->field_3a = obj->field_3a & 0xff00;
            p = func_8011f0e8();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0x14;
                p->field_03 = 2;
                p->field_ad = 0;
                p->field_5c = 0;
                p->field_66 = obj->field_66;
                p->field_65 = obj->field_65;
                p->field_ac = (obj->field_12a >> 1) + 3;
                p->field_0b = obj->field_0b;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->field_0d = obj->field_0d;
                p->field_26 = obj->field_26;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = ((Slot04bObj *)obj)->field_70;
                p->field_7a = 0x60;
                p->field_3c = obj;
                p->field_7c = 0x1e0;
                ((Slot04bObj *)p)->field_70 = t;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                obj->field_14c = (s32)p;
                obj->field_240++;
                func_801204f4(obj, obj->side, 0xb);
            }
        }
    }
}
