/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c74b8_slot04_11[];
extern u8 data_801c74c8_slot04_11[];
extern u8 data_801c74d0_slot04_11[];
extern ObjectFn data_801c74dc_slot04_11[];

void func_80142adc(Object *object);
Block172 *func_8011f1e0(void);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801b2e38_slot04_11(Object *o);
void func_801b5874_slot04_11(Object *obj);
void func_801b5894_slot04_11(Object *obj);
int func_801b5764_slot04_11(Object *obj);

void func_801b29c0_slot04_11(Object *obj) {
    s16 t;
    int a;
    u8 s;

    if (func_801b5764_slot04_11(obj) < 0) {
        t = obj->field_70;
        if (obj->pos_y >= t) {
            obj->pos_y = t;
            func_801209c4(obj);
            a = 0x37;
            s = obj->field_48;
            obj->field_14 = 0;
            obj->field_45 = 0;
            obj->field_07 = 8;
            obj->field_17b = 0;
            if (!(s & 0x80)) {
                a = (s & 0x7f) + 0x38;
            }
            func_801307e0(obj, a);
        }
    }
    if (obj->field_67 != 0) {
        a = obj->field_48 + 0x35;
        obj->field_67 = 0;
        obj->field_48 |= 0x80;
        func_801307e0(obj, a);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2a84_slot04_11(Object *obj) {
    if (func_801b5764_slot04_11(obj) < 0 && obj->pos_y >= obj->field_70) {
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

void func_801b2b10_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5894_slot04_11(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b2b84_slot04_11(Object *obj) {
    data_801c74b8_slot04_11[obj->field_07](obj);
}

void func_801b2bc4_slot04_11(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x50);
}

void func_801b2c28_slot04_11(Object *obj) {
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

void func_801b2c88_slot04_11(Object *o) {
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
        t = data_801c74c8_slot04_11[k & 0xfe];
        o->field_165 = 0;
        o->field_27b = t;
    }
    if (o->field_3a & 2) {
        o->field_3a &= 0xfffd;
        func_801483a4(o, -6, 0x4f);
        func_80120554(o, o->side, 0x31c);
    }
}

void func_801b2d64_slot04_11(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_67 = 0;
    }
    if (obj->field_67 != 0) {
        obj->field_67 = 0;
        ref_other.p = obj->other;
        if (ref_other.p->field_61 != 0xff && (s16)ref_other.p->field_5c >= 0) {
            func_801b2e38_slot04_11(obj);
        }
    }
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2e38_slot04_11(Object *o) {
    Object *p;
    int i;
    u8 c;

    func_801204f4(o, o->side, 10);
    ref_other.p->field_290 = 0;
    i = (u8)(o->field_12a << 1);
    c = data_801c74d0_slot04_11[i];
    ref_other.p->field_28e = data_801c74d0_slot04_11[i + 2];
    ref_other.p->field_290 = (ref_other.p->field_290 & 0xff00) | data_801c74d0_slot04_11[i + 3];
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
            p->field_02 = 0x16;
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

void func_801b300c_slot04_11(Object *obj) {
    data_801c74dc_slot04_11[obj->field_07](obj);
}

void func_801b304c_slot04_11(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, obj->field_12a + 0x53);
}

void func_801b30ac_slot04_11(Object *obj) {
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
