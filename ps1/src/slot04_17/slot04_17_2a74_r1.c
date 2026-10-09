/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ce3ec_slot04_17[];
extern u8 data_801ce3fc_slot04_17[];
extern u8 data_801ce404_slot04_17[];
extern ObjectFn data_801ce410_slot04_17[];
extern s32 data_801ce428_slot04_17[];
extern u8 data_801ce42c_slot04_17[][4];
extern u16 data_801ce448_slot04_17[];

int func_8013fd98(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_80142adc(Object *object);
Block172 *func_8011f1e0(void);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801b2e28_slot04_17(Object *o);
void func_801b57f0_slot04_17(Object *obj);
void func_801b5810_slot04_17(Object *obj);
int func_801b56e0_slot04_17(Object *obj);
void func_801b328c_slot04_17(Object *obj);

void func_801b2a74_slot04_17(Object *obj) {
    if (func_801b56e0_slot04_17(obj) < 0 && obj->pos_y >= obj->field_70) {
        func_801209c4(obj);
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        obj->field_07++;
        func_801307e0(obj, 0x37);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2b00_slot04_17(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5810_slot04_17(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b2b74_slot04_17(Object *obj) {
    data_801ce3ec_slot04_17[obj->field_07](obj);
}

void func_801b2bb4_slot04_17(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x50);
}

void func_801b2c18_slot04_17(Object *obj) {
    int v;

    func_80130efc(obj);
    v = -1;
    if (obj->field_3a & 1) {
        obj->field_67 = 0;
        obj->field_12f = 0;
        obj->field_07++;
        if (obj->field_4b != 0) {
            v = 1;
        }
        obj->field_165 = v;
    }
}

void func_801b2c78_slot04_17(Object *o) {
    int k;
    u8 t;

    func_80130efc(o);
    k = 6;
    if (!(o->field_3a & 1)) {
        o->field_07++;
        o->field_3a &= 0xfffe;
        if (o->field_4b == 0) {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            k = o->field_12a;
        }
        t = data_801ce3fc_slot04_17[k & 0xfe];
        o->field_165 = 0;
        o->field_27b = t;
    }
    if (o->field_3a & 2) {
        o->field_3a &= 0xfffd;
        func_801483a4(o, -6, 0x4f);
        func_80120554(o, o->side, 0x31c);
    }
}

void func_801b2d54_slot04_17(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_67 = 0;
    }
    if (obj->field_67 != 0) {
        obj->field_67 = 0;
        ref_other.p = obj->other;
        if (ref_other.p->field_61 != 0xff && (s16)ref_other.p->field_5c >= 0) {
            func_801b2e28_slot04_17(obj);
        }
    }
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2e28_slot04_17(Object *o) {
    Object *p;
    int i;
    u8 c;

    func_801204f4(o, o->side, 10);
    ref_other.p->field_290 = 0;
    i = (u8)(o->field_12a << 1);
    c = data_801ce404_slot04_17[i];
    ref_other.p->field_28e = data_801ce404_slot04_17[i + 2];
    ref_other.p->field_290 = (ref_other.p->field_290 & 0xff00) | data_801ce404_slot04_17[i + 3];
    if (o->field_3c == 0) {
        ref_other.p->field_28c = c;
        ref_other.p->field_28d = 0x3c;
        ref_other.p->field_297 = 0x3c;
        p = (Object *)func_8011f1e0();
        if (p == 0) {
            goto fail;
        }
        {
            p->field_00 = 1;
            p->field_02 = 0x5e;
            p->other = o->other;
            p->field_3c = o;
            o->field_3c = p;
            p->field_0e = o->field_0e;
            p->field_09 = 1;
            p->field_1c = o->field_1c;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = o->field_0d;
            p->field_90 = o->field_90;
            p->field_98 = o->field_98;
            p->field_9c = o->field_9c;
            p->field_08 = 0x20;
            p->field_02 = 0x18;
            p->field_66 = o->field_66;
        }
    } else {
        ref_other.p->field_297 += 0xf6;
        if (ref_other.p->field_297 == 0 || (ref_other.p->field_297 & 0x80)) {
        fail:
            ref_other.p->field_297 = 9;
        }
    }
}

void func_801b2ffc_slot04_17(Object *obj) {
    data_801ce410_slot04_17[obj->field_07](obj);
}

void func_801b303c_slot04_17(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, obj->field_12a + 0x53);
}

void func_801b309c_slot04_17(Object *obj) {
    int v;

    func_80130efc(obj);
    v = -1;
    if (obj->field_3a & 1) {
        obj->field_10 = 0;
        obj->field_67 = 0;
        obj->field_12f = 0;
        obj->field_07++;
        obj->field_0b = obj->field_158;
        if (obj->field_4b != 0) {
            v = 1;
        }
        obj->field_165 = v;
    }
}

void func_801b3108_slot04_17(Object *obj) {
    int i;
    u8 t;

    func_80130efc(obj);
    if ((obj->field_3a & 1) == 0) {
        if (obj->field_165 != 0) {
            i = 6;
            if (obj->field_4b == 0) {
                ref_other.p = obj->other;
                ref_other.p->field_6b = 10;
                i = obj->field_12a;
            }
            t = data_801ce42c_slot04_17[i >> 1][0];
            obj->field_165 = 0;
            obj->field_27b = t;
        }
    }
    if (obj->field_3a & 2) {
        obj->field_3a &= 0xfffd;
        func_801483a4(obj, 0x28, 0x66);
        func_80120554(obj, obj->side, 0x31c);
    }
    if (obj->field_3a & 4) {
        int d;
        u16 w;

        w = obj->field_3a & 0xfffb;
        obj->field_3a = w;
        d = (w >> 8) & 0x7f;
        if (obj->field_0b == 0) {
            d = -d;
        }
        obj->pos_x += d;
    }
    if (obj->field_3a & 8) {
        obj->field_07++;
        obj->field_4c = data_801ce428_slot04_17[obj->field_12a];
        obj->field_54 = data_801ce428_slot04_17[obj->field_12a + 1] & 0xffff0000;
        *(u16 *)&obj->field_54 = obj->pos_x;
        func_801b328c_slot04_17(obj);
    }
}

void func_801b328c_slot04_17(Object *obj) {
    Object *p;
    int a;
    int k;
    int b;
    int q;
    int r;
    int t;

    a = obj->field_4c;
    p = obj->other;
    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
    if (obj->field_67 != 0) {
        ref_other.p = obj->other;
        if (p->field_163 == 0 && *(u16 *)&p->field_04 == 0x101 &&
            (u8)func_8013fd98(obj, 0, 0x40, 8, 8) != 0) {
            obj->field_4c = 0xc0000;
            obj->field_54 = -0x8000;
            obj->field_07++;
            ref_other.p->field_261 = 1;
            ((Slot04bObj *)obj)->field_52 = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->field_45 = 0;
            if ((s16)ref_other.p->field_5c < 0) {
                obj->field_12f = 0xff;
            }
            func_801307e0(obj, obj->field_12a + 0x54);
            return;
        }
    }
    a = obj->pos_x;
    a -= (s16)obj->field_54;
    k = ((Slot04bObj *)obj)->field_56;
    a += k;
    k <<= 1;
    if (!((u32)k < (u32)a)) {
        a = data_801ce448_slot04_17[obj->field_0b];
        if (!(a & obj->field_164)) {
            func_80130efc(obj);
            return;
        }
    }
    p = obj->other;
    obj->field_07 += 2;
    q = obj->pos_x;
    b = obj->field_0b;
    r = p->pos_x;
    k = -1;
    if (b == 0) {
        t = q;
        q = r;
        r = t;
        k = 1;
    }
    if (!(q < r)) {
        if (data_801ce448_slot04_17[b] & ref_other.p->field_164) {
            ref_other.p->pos_x += k;
            ref_other.p->field_164 = 0;
        }
    }
    obj->field_4c = 0x80000;
    obj->field_54 = 0xffff0000;
    func_801307e0(obj, 0x59);
}
