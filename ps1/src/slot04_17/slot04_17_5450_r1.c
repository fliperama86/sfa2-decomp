/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ce76c_slot04_17[];
extern Object *data_801ce778_slot04_17[];
extern ObjectFn data_801ce788_slot04_17[];
extern Slot04_17Rece790 data_801ce790_slot04_17[];
extern ObjectFn data_801ce844_slot04_17[];
extern ObjectFn data_801ce850_slot04_17[];
extern ObjectFn data_801ce858_slot04_17[];
extern u8 data_801ce860_slot04_17[];
extern ObjectFn data_801ce8f0_slot04_17[];
extern ObjectFn data_801ce900_slot04_17[];
extern Slot04_17Recec3c data_801cec3c_slot04_17[];

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80141e5c(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80130dc0(Object *object);
void func_80131468(Object *object);
void func_80131638(Object *object);
void func_80130678(Object *object, int arg);
unsigned short func_80130470(Object *object);
void func_80142a14(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);
void func_80141f28(Object *object, short delta);

void func_801b57f0_slot04_17(Object *obj);
Object *func_801b5890_slot04_17(Object *obj);
int func_801b5850_slot04_17(Object *obj);
int func_801b5870_slot04_17(Object *obj);
void func_801b5a34_slot04_17(Object *obj);
void func_801b5ee0_slot04_17(Object *obj);
void func_801b5f04_slot04_17(Object *obj);
int func_801b5f44_slot04_17(Object *obj);
void func_801b5fc8_slot04_17(Object *obj);
void func_801b656c_slot04_17(Object *obj);
void func_801b65bc_slot04_17(Object *obj);
void func_801b67f0_slot04_17(Object *obj);
void func_801b6854_slot04_17(Object *obj);
Object *func_801b68c4_slot04_17(Object *obj);
void func_801b6198_slot04_17(Object *obj);

void func_801b5450_slot04_17(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5494_slot04_17(Object *obj) {
    data_801ce76c_slot04_17[obj->field_07](obj);
}

void func_801b54d4_slot04_17(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801204f4(obj, obj->side, 8);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (func_801b5890_slot04_17(obj)->pos_x + 0xc0 >= obj->pos_x) {
            goto inc;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
inc:
        obj->field_0b++;
    }
    func_801307e0(obj, 0x1b);
    func_80141e5c(obj);
}

void func_801b55a0_slot04_17(Object *obj) {
    if (obj->field_3a & 4) {
        obj->field_07++;
        func_80140770(obj, 8, 5, 0xf, 0, 0, 1);
    } else if (obj->field_3a & 2) {
        obj->field_3a = obj->field_3a & 0xfffd;
        func_80146478(obj, obj->field_12a >> 1, -0x59, 0x1e);
        func_80120554(obj, obj->side ^ 1, 0x30d);
        func_801204f4(obj, obj->side, 6);
    }
    func_80130efc(obj);
    if (obj->field_3a & 1) {
        func_80141e5c(obj);
    }
}

void func_801b5678_slot04_17(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b56bc_slot04_17(Object *obj) {
    obj->field_159 = 1;
    func_80130dc0(obj);
}

int func_801b56e0_slot04_17(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_4c += obj->field_54;
    return obj->field_50 += obj->field_58;
}

int func_801b5724_slot04_17(Object *obj) {
    s32 a = obj->field_4c;

    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    return obj->field_50 = obj->field_50 + obj->field_58;
}

int func_801b5788_slot04_17(Object *obj) {
    if (obj->kind == 0x11) {
        if (obj->flags_28b & 1) {
            goto y;
        }
x:
        return func_801b5870_slot04_17(obj);
    } else if (obj->flags_28b & 1) {
        goto x;
    }
y:
    return func_801b5850_slot04_17(obj);
}

void func_801b57f0_slot04_17(Object *obj) {
    func_801312b8(obj);
}

void func_801b5810_slot04_17(Object *obj) {
    func_80131468(obj);
}

void func_801b5830_slot04_17(Object *obj) {
    func_80131638(obj);
}

int func_801b5850_slot04_17(Object *obj) {
    return ((obj->field_134 | obj->field_136) & 0x94) == 0x94;
}

int func_801b5870_slot04_17(Object *obj) {
    return ((obj->field_134 | obj->field_136) & 0x68) == 0x68;
}

Object *func_801b5890_slot04_17(Object *obj) {
    return data_801ce778_slot04_17[obj->field_0e];
}

void func_801b58b4_slot04_17(Object *obj) {
    if (obj->kind == 0x11 || obj->kind == 0x13) {
        data_801ce788_slot04_17[obj->field_07](obj);
    }
}

void func_801b590c_slot04_17(Object *obj) {
    obj->field_07++;
    obj->flags_28b = 0;
    obj->kind ^= 2;
    func_801b5a34_slot04_17(obj);
    select_box_tables(obj);
    build_metrics(obj);
    func_80130678(obj, 0x30);
}

void func_801b596c_slot04_17(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
        return;
    }
    if (obj->field_cd == 0) {
        if (func_80130470(obj) & 0xffff) {
            func_8013047c(obj);
            return;
        }
        if (func_8012f970(obj)) {
            func_8012fe60(obj);
            return;
        }
    }
    if ((u8)func_8012f898(obj)) {
        func_8012f8c4(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5a34_slot04_17(Object *obj) {
    u32 *dst;
    Slot04_17Rece790 *p;
    int k;
    int i;

    k = 2;
    if (obj->kind == 0x11) {
        k = 1;
    }
    dst = (u32 *)0x1f800100;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    for (i = 0; i < 15; i++) {
        p = &data_801ce790_slot04_17[i];
        dst[data_801ce790_slot04_17[i].a] = (&p->a)[k];
    }
}

void func_801b5ab8_slot04_17(Object *obj) {
    data_801ce844_slot04_17[obj->field_128 >> 1](obj);
}

void func_801b5afc_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801ce850_slot04_17[obj->field_07](obj);
}

void func_801b5b3c_slot04_17(Object *obj) {
    obj->field_07++;
    if (obj->field_12a == 0) {
        func_801b5ee0_slot04_17(obj);
    } else if (obj->field_25f == 0 && (obj->field_130 & 0xa000) && (func_8013f8c4(obj, -0x14, 0xe) & 0xff)) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b5ee0_slot04_17(obj);
        if (obj->field_129 == 0 && obj->field_12a != 0) {
            obj->field_278 = 1;
            obj->field_29a = 1;
        }
        func_801b5f04_slot04_17(obj);
    }
}

void func_801b5c10_slot04_17(Object *obj) {
    func_80142a14(obj);
    func_801b5f04_slot04_17(obj);
}

void func_801b5c40_slot04_17(Object *obj) {
    obj->field_157 = 1;
    data_801ce858_slot04_17[obj->field_07](obj);
}

void func_801b5c84_slot04_17(Object *obj) {
    obj->field_07++;
    func_801b5ee0_slot04_17(obj);
    if (obj->field_129 != 0) {
        if (obj->field_12a == 4) {
            obj->field_278 = 1;
            func_801b5f04_slot04_17(obj);
        }
    } else if (obj->field_12a == 4) {
        obj->field_29c = 1;
        obj->field_29a = 1;
    }
}

void func_801b5d08_slot04_17(Object *obj) {
    func_80142a14(obj);
    func_801b5f04_slot04_17(obj);
}

void func_801b5d38_slot04_17(Object *obj) {
    if (obj->field_67 != 0 && *(u8 *)&obj->field_3a != 0 && (obj->field_134 & 8)) {
        func_801307e0(obj, obj->field_48 != 0 ? 0x19 : 0x18);
    } else if (func_801b5f44_slot04_17(obj) < 0 && obj->pos_y >= obj->field_70) {
        func_801b5fc8_slot04_17(obj);
        func_801209c4(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b5dfc_slot04_17(Object *obj) {
    s16 n;
    s16 t;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, *(s16 *)((u8 *)data_801ce860_slot04_17 + (obj->field_12a & 0xfe)));
    n = 0xc;
    if (obj->field_48 != 0) {
        n = 0x12;
    }
    if (obj->field_129 != 0) {
        n += 3;
    }
    t = obj->field_12a >> 1;
    t += n;
    func_801307e0(obj, t);
    if (obj->field_129 != 0 && obj->field_12a == 4) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 5;
        obj->field_07 = 0;
        obj->field_67 = 0;
    }
}

void func_801b5ee0_slot04_17(Object *obj) {
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b5f04_slot04_17(Object *obj) {
    u16 t = obj->field_3a;
    s16 d = t & 0x7f;
    s16 e = d;

    if (d != 0) {
        obj->field_3a = t & 0xff80;
        if (obj->field_0b == 0) {
            e = -d;
        }
        obj->pos_x = e + obj->pos_x;
    }
}

int func_801b5f44_slot04_17(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_4c += obj->field_54;
    return obj->field_50 += obj->field_58;
}

void func_801b5f88_slot04_17(Object *obj) {
    func_801312b8(obj);
}

void func_801b5fa8_slot04_17(Object *obj) {
    func_80131468(obj);
}

void func_801b5fc8_slot04_17(Object *obj) {
    func_80131638(obj);
}

void func_801b5fe8_slot04_17(Object *obj) {
    ref_other.p = obj->other;
    data_801ce8f0_slot04_17[obj->field_04](obj);
}

void func_801b6038_slot04_17(Object *obj) {
    Object *o;

    obj->field_01 = 1;
    obj->field_98 = data_80172a48;
    obj->field_90 = (void *)0x800fb100;
    obj->field_04++;
    obj->field_9c = data_80173c9c;
    func_801b656c_slot04_17(obj);
    o = ref_other.p->other;
    func_801204f4(o, o->side, 0xf);
    ((Slot04bObj *)obj)->field_47 = ref_other.p->field_28c;
    func_801b65bc_slot04_17(obj);
    func_801b67f0_slot04_17(obj);
}

void func_801b60dc_slot04_17(Object *obj) {
    func_801b656c_slot04_17(obj);
    if ((*(u32 *)&ref_other.p->field_28c & 0x80ff) != 0x8000 && (game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) == 0) {
        data_801ce900_slot04_17[obj->field_05](obj);
        func_801b6198_slot04_17(obj);
    } else {
        obj->field_04++;
        func_801b6854_slot04_17(obj);
    }
}

void func_801b6198_slot04_17(Object *obj) {
    Slot04_17Recec3c *r = &data_801cec3c_slot04_17[obj->other->side];
    Object *t = func_801b68c4_slot04_17(ref_other.p);

    r->c = obj->box_tables;
    r->x = obj->pos_x - t->pos_x - 8;
    r->y = obj->pos_y + t->pos_y;
    func_801519b4((Object *)r);
}
