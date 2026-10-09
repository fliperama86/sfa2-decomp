/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3284_slot04_15[];
extern u8 data_801c3c24_slot04_15[];
extern s32 data_801c3c2c_slot04_15[];
extern u8 data_801c3c44_slot04_15[];
extern ObjectFn data_801c3c4c_slot04_15[];
extern ObjectFn data_801c3c5c_slot04_15[];
extern ObjectFn data_801c3c70_slot04_15[];
extern ObjectFn data_801c3c78_slot04_15[];
extern ObjectFn data_801c3c80_slot04_15[];
extern ObjectFn data_801c3c90_slot04_15[];
extern ObjectFn data_801c3ca0_slot04_15[];
extern Object *data_801c3d7c_slot04_15;
extern BoxTables *data_801c34e4_slot04_15[];
extern BoxTables *data_801c3c08_slot04_15[];
extern BoxTables *data_801c3810_slot04_15[];
extern SequenceStep *data_801c2ca8_slot04_15[];
extern SequenceStep *data_801c0fc4_slot04_15[];

void func_8011f14c(Slab172 *o);
void func_8011f38c(Object *o);
void func_8011ffdc(Object *o);
void func_80137b64(Object *object);
void func_80137be0(Object *object);
void func_80137cc0(Object *object);
void func_80137dc8(Object *object);
void func_80137f00(Object *object);

void func_801b3d14_slot04_15(Object *object);
void func_801b3d48_slot04_15(Object *object);
void func_801b3f8c_slot04_15(Object *object);
void func_801b40cc_slot04_15(Object *object);
void func_801b42b0_slot04_15(Object *obj);
void func_801b4308_slot04_15(Object *o, u8 a);
void func_801b43f4_slot04_15(Object *obj);
void func_801b41fc_slot04_15(Object *obj);

void func_801b3b74_slot04_15(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x4f; i >= 0; i--) {
        *p++ = z;
    }
}

void func_801b3b98_slot04_15(Object *object) {
    data_801c3c4c_slot04_15[object->field_04](object);
}

void func_801b3bd8_slot04_15(Object *object) {
    Object *parent;
    int dx;
    int i;

    parent = object->field_3c;
    object->field_09 = 0;
    object->field_04 = object->field_04 + 1;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    object->field_1e = parent->field_1e;
    ((Slot04bObj *)object)->field_6c = data_801c3284_slot04_15;
    if (object->field_03 != 0) {
        func_801b40cc_slot04_15(object);
    }
    object->field_45 = 0;
    object->field_50 = 0;
    i = object->field_ac >> 1;
    i = data_801c3c2c_slot04_15[i];
    dx = 0x59;
    if (object->field_0b == 0) {
        i = -i;
        dx = -0x59;
    }
    object->field_4c = i;
    object->pos_x = object->pos_x + dx;
    object->pos_y = object->pos_y - 0x36;
    i = object->field_ac >> 1;
    object->field_a0 = data_801c3c24_slot04_15[i];
    if (parent->side == 0) {
        ((Slot04bObj *)object)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)object)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(object, i);
    func_801b3d14_slot04_15(object);
    func_801b3d48_slot04_15(object);
}

void func_801b3d14_slot04_15(Object *object) {
    object->field_46 = (s8)data_801c3c44_slot04_15[object->field_ac >> 1] | (object->field_46 & 0xff00);
}

void func_801b3d48_slot04_15(Object *object) {
    data_801c3c5c_slot04_15[object->field_05](object);
}

void func_801b3d88_slot04_15(Object *object) {
    u16 t;

    if ((game_state.field_65 | game_state.field_a8) != 0) {
        goto call;
    }
    if (object->field_03 != 0) {
        if ((s16)object->field_3a >= 0) {
            goto call;
        }
        goto set;
    }
    t = object->field_46 - 1;
    object->field_46 = t;
    if ((u8)t == 0) {
        goto set;
    }
call:
    func_80137b64(object);
    return;
set:
    object->field_04 = 2;
    object->field_05 = 0;
    object->field_06 = 0;
    object->field_07 = 0;
    func_801b3f8c_slot04_15(object);
}

void func_801b3e20_slot04_15(Object *object) {
    if ((game_state.field_65 | game_state.field_a8) != 0) {
        func_80137be0(object);
    } else {
        data_801c3c70_slot04_15[object->field_06](object);
    }
}

void func_801b3e88_slot04_15(Object *object) {
    object->field_06 = object->field_06 + 1;
    func_801b40cc_slot04_15(object);
    ((Slot04bObj *)object)->field_46 = 2;
    func_80131094(object);
}

void func_801b3ed0_slot04_15(Object *object) {
    ((Slot04bObj *)object)->field_46--;
    if (((Slot04bObj *)object)->field_46 == 0) {
        object->field_00 = 1;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
}

void func_801b3f0c_slot04_15(Object *object) {
    func_80137cc0(object);
}

void func_801b3f2c_slot04_15(Object *object) {
    func_801b3d14_slot04_15(object);
    func_80137dc8(object);
}

void func_801b3f5c_slot04_15(Object *object) {
    func_801b3d14_slot04_15(object);
    func_80137f00(object);
}

void func_801b3f8c_slot04_15(Object *obj) {
    data_801c3c78_slot04_15[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801b3fe0_slot04_15(Object *obj) {
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

void func_801b4040_slot04_15(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 3;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
    func_80131094(obj);
}

void func_801b4080_slot04_15(Object *o) {
    Object *p = o->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b40cc_slot04_15(Object *obj) {
    BoxTables **t;

    if (obj->field_ac == 6) {
        t = data_801c34e4_slot04_15;
    } else {
        t = data_801c3c08_slot04_15;
        if (obj->field_ac == 8) {
            t = data_801c3810_slot04_15;
        }
    }
    t = (BoxTables **)t[(s16)obj->field_5c];
    obj->box_tables = (BoxTables *)t;
}

void func_801b4120_slot04_15(Object *obj) {
    data_801c3d7c_slot04_15 = obj->field_3c;
    data_801c3c80_slot04_15[obj->field_04](obj);
}

void func_801b4170_slot04_15(Object *obj) {
    Object *p;

    obj->field_04++;
    p = data_801c3d7c_slot04_15;
    obj->field_1c = p->field_1c;
    obj->field_1e = p->field_1e;
    obj->field_03 = p->kind;
    obj->field_0c = data_801c3d7c_slot04_15->field_0c;
    obj->field_0d = data_801c3d7c_slot04_15->field_0d;
    obj->field_48 = 0;
    func_801b41fc_slot04_15(obj);
}

void func_801b41fc_slot04_15(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801c3d7c_slot04_15;
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
                func_801b4308_slot04_15(obj, a);
            } else {
                func_80131094(obj);
            }
        }
    } else {
        func_801b42b0_slot04_15(obj);
    }
}

void func_801b42b0_slot04_15(Object *obj) {
    obj->field_04++;
}

void func_801b42c4_slot04_15(Object *o) {
    Object *p = data_801c3d7c_slot04_15;

    if ((s32)o == p->field_28) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}

void func_801b4300_slot04_15(Object *obj) {
    obj->field_48 = 0;
}

void func_801b4308_slot04_15(Object *o, u8 a) {
    o->field_48 = a;
    func_80130700(o, data_801c2ca8_slot04_15[a]);
}

void func_801b4344_slot04_15(Object *obj) {
    data_801c3c90_slot04_15[obj->field_04](obj);
}

void func_801b4384_slot04_15(Object *obj) {
    obj->field_50 = 0xfffe0000;
    obj->field_58 = 0x1000;
    obj->field_04 = obj->field_04 + 1;
    obj->field_4c = (*(s32 *)&obj->field_3c->field_10 - *(s32 *)&obj->field_10) >> 6;
    func_80130768(obj, 0, data_801c0fc4_slot04_15);
    func_801b43f4_slot04_15(obj);
}

void func_801b43f4_slot04_15(Object *obj) {
    if (game_state.field_6a != 0) {
        obj->field_04 = 2;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        data_801c3ca0_slot04_15[obj->field_05](obj);
        func_8011ffdc(obj);
    }
}

void func_801b446c_slot04_15(Object *obj) {
    s16 t;

    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    t = (u16)obj->field_3c->field_70;
    t -= 0x55;
    if (obj->pos_y < t) {
        func_80131094(obj);
    } else {
        obj->field_05 = obj->field_05 + 1;
        obj->field_46 = (obj->field_46 & 0xff00) | 4;
    }
}

void func_801b4508_slot04_15(Object *obj) {
    u16 t = obj->field_46;
    t -= 1;
    obj->field_46 = t;
    if ((u8)t == 0) {
        obj->field_50 = 0xffff0000;
        obj->field_58 = 0x2000;
        obj->field_05 = obj->field_05 + 1;
    }
}
