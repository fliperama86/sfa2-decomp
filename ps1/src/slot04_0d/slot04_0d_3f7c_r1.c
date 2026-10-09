/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c35f8_slot04_0d[];
extern ObjectFn data_801c3600_slot04_0d[];
extern ObjectFn data_801c3610_slot04_0d[];
extern ObjectFn data_801c361c_slot04_0d[];
extern ObjectFn data_801c3624_slot04_0d[];
extern ObjectFn data_801c3630_slot04_0d[];
extern ObjectFn data_801c363c_slot04_0d[];
extern Object *data_801c36fc_slot04_0d;
extern BoxTables *data_801c2e60_slot04_0d[];
extern BoxTables *data_801c3584_slot04_0d[];
extern BoxTables *data_801c318c_slot04_0d[];
extern SequenceStep *data_801c2624_slot04_0d[];

void func_8011f14c(Slab172 *o);
void func_8011f38c(Object *o);
void func_8011ffdc(Object *o);
void func_80130dc0(Object *obj);
void func_801b42a0_slot04_0d(Object *obj);
void func_801b41ec_slot04_0d(Object *obj);
void func_801b42f8_slot04_0d(Object *obj, u8 a);
void func_801b43b4_slot04_0d(Object *obj);
void func_801b46d4_slot04_0d(Object *obj);
void func_801b4374_slot04_0d(Object *obj);
void func_801b490c_slot04_0d(Object *obj);

void func_801b3f7c_slot04_0d(Object *obj) {
    data_801c35f8_slot04_0d[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801b3fd0_slot04_0d(Object *obj) {
    u8 t;

    obj->field_05++;
    obj->field_3c->field_14c = 0;
    if (obj->field_03 != 0) {
        t = obj->field_3a + 6;
    } else {
        t = (obj->field_ac >> 1) + 6;
    }
    func_80138070(obj, t);
}

void func_801b4030_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 3;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
    func_80131094(obj);
}

void func_801b4070_slot04_0d(Object *o) {
    Object *p = o->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b40bc_slot04_0d(Object *obj) {
    BoxTables **t;

    if (obj->field_ac == 6) {
        t = data_801c2e60_slot04_0d;
    } else {
        t = data_801c3584_slot04_0d;
        if (obj->field_ac == 8) {
            t = data_801c318c_slot04_0d;
        }
    }
    t = (BoxTables **)t[(s16)obj->field_5c];
    obj->box_tables = (BoxTables *)t;
}

void func_801b4110_slot04_0d(Object *obj) {
    data_801c36fc_slot04_0d = obj->field_3c;
    data_801c3600_slot04_0d[obj->field_04](obj);
}

void func_801b4160_slot04_0d(Object *obj) {
    Object *p;

    obj->field_04++;
    p = data_801c36fc_slot04_0d;
    obj->field_1c = p->field_1c;
    obj->field_1e = p->field_1e;
    obj->field_03 = p->kind;
    obj->field_0c = data_801c36fc_slot04_0d->field_0c;
    obj->field_0d = data_801c36fc_slot04_0d->field_0d;
    obj->field_48 = 0;
    func_801b41ec_slot04_0d(obj);
}

void func_801b41ec_slot04_0d(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801c36fc_slot04_0d;
    if (obj->field_03 == p->kind) {
        a = p->frame->field_09;
        if (a == 0) {
            obj->field_48 = 0;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&p->field_10;
            *(s32 *)&obj->field_14 = *(s32 *)&p->field_14;
            obj->field_0b = p->field_0b;
            obj->field_01 = 1;
            if (obj->field_48 != a) {
                func_801b42f8_slot04_0d(obj, a);
            } else {
                func_80131094(obj);
            }
        }
    } else {
        func_801b42a0_slot04_0d(obj);
    }
}

void func_801b42a0_slot04_0d(Object *obj) {
    obj->field_04++;
}

void func_801b42b4_slot04_0d(Object *o) {
    Object *p = data_801c36fc_slot04_0d;

    if ((s32)o == p->field_28) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}

void func_801b42f0_slot04_0d(Object *obj) {
    obj->field_48 = 0;
}

void func_801b42f8_slot04_0d(Object *o, u8 a) {
    o->field_48 = a;
    func_80130700(o, data_801c2624_slot04_0d[a]);
}

void func_801b4334_slot04_0d(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b490c_slot04_0d(obj);
    } else {
        func_801b4374_slot04_0d(obj);
    }
}

void func_801b4374_slot04_0d(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b46d4_slot04_0d(obj);
    } else {
        func_801b43b4_slot04_0d(obj);
    }
}

void func_801b43b4_slot04_0d(Object *obj) {
    data_801c3610_slot04_0d[obj->field_12a >> 1](obj);
}

void func_801b43f8_slot04_0d(Object *object) {
    data_801c361c_slot04_0d[object->field_07](object);
}

void func_801b4438_slot04_0d(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if ((u8)func_8013f8c4(obj, -0x11, 0x14) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b44c8_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b452c_slot04_0d(Object *object) {
    data_801c3624_slot04_0d[object->field_07](object);
}

void func_801b456c_slot04_0d(Object *obj) {
    u8 one;

    obj->field_07++;
    if (obj->field_218 != 0) {
        if ((u8)func_8013f8c4(obj, -0x11, 0x14) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        }
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        obj->field_278 = one;
    }
}

void func_801b45f0_slot04_0d(Object *obj) {
    s16 d;

    if (obj->field_3a & 0xff) {
        obj->field_07++;
        d = -0x18;
        if (obj->field_0b != 0) {
            d = 0x18;
        }
        *(s32 *)&obj->field_10 += d;
    }
    if (func_80149b80(obj) & 0xff) {
        obj->field_07 = 0;
    }
    func_80130efc(obj);
}

void func_801b4670_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b46d4_slot04_0d(Object *obj) {
    data_801c3630_slot04_0d[obj->field_12a >> 1](obj);
}

void func_801b4718_slot04_0d(Object *object) {
    data_801c363c_slot04_0d[object->field_07](object);
}
