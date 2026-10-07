/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4c10_slot04_12[];
extern s32 data_801c4c28_slot04_12[];
extern u8 data_801c4c58_slot04_12[];
extern ObjectFn data_801c4c5c_slot04_12[];
extern u8 data_801c4c74_slot04_12[];
Object *func_8011f0e8(void);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
int func_801b3a90_slot04_12(Object *object);

void func_801b276c_slot04_12(Object *obj) {
    data_801c4c10_slot04_12[obj->field_07](obj);
}

void func_801b27ac_slot04_12(Object *o) {
    o->field_07++;
    o->field_46 = ((Slot04bObj *)o)->field_46 + 0x400;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    if (o->field_0b != 0) {
        o->pos_x = o->pos_x + 0x18;
    } else {
        o->pos_x = o->pos_x - 0x18;
    }
    o->field_4c = data_801c4c28_slot04_12[o->field_12a * 2];
    o->field_54 = data_801c4c28_slot04_12[o->field_12a * 2 + 1];
    o->field_50 = data_801c4c28_slot04_12[o->field_12a * 2 + 2];
    o->field_58 = data_801c4c28_slot04_12[o->field_12a * 2 + 3];
    func_801307e0(o, o->field_12a + 0x53);
}

void func_801b28b8_slot04_12(Object *o) {
    o->field_46 = (s16)o->field_46 - 0x100;
    if (o->field_46 & 0xff00) {
        o->field_07++;
        o->field_46 = ((Slot04bObj *)o)->field_46 + 0x3800;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        func_801204f4(o, o->side, 6);
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0xc, 0x30);
    }
}

void func_801b295c_slot04_12(Object *o) {
    o->field_46 = (s16)o->field_46 - 0x100;
    if ((o->field_46 & 0xff00) == 0) {
        o->field_07++;
        if (o->field_4b != 0) {
            ((Slot04bObj *)o)->field_27b = data_801c4c58_slot04_12[3];
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            ((Slot04bObj *)o)->field_27b = data_801c4c58_slot04_12[o->field_12a >> 1];
        }
        o->field_165 = 0;
        func_801204f4(o, o->side, 7);
    }
}

void func_801b2a08_slot04_12(Object *o) {
    if (*(u8 *)&o->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
    }
    func_80130efc(o);
}

void func_801b2a48_slot04_12(Object *o) {
    if (func_801b3a90_slot04_12(o) < 0 && ((s16)o->field_3a & 0x8000)) {
        o->field_58 = -0x6000;
        if (o->pos_y > o->field_70) {
            o->field_14 = 0;
            o->field_45 = 0;
            o->field_159 = 0;
            o->field_07++;
            o->pos_y = o->field_70;
            func_801209c4(o);
            func_801307e0(o, o->field_12a + 0x54);
            return;
        }
    }
    func_80130efc(o);
}

void func_801b2aec_slot04_12(Object *o) {
    if (((s16)o->field_3a & 0x8000) == 0) {
        func_80130efc(o);
    } else {
        ref_other.p = o->other;
        ref_other.p->field_249 = 5;
        func_801312b8(o);
    }
}

void func_801b2b4c_slot04_12(Object *obj) {
    data_801c4c5c_slot04_12[obj->field_07](obj);
}

void func_801b2b8c_slot04_12(Object *o) {
    o->field_07++;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    func_801307e0(o, 0x59);
}

void func_801b2be8_slot04_12(Object *o) {
    func_80130efc(o);
    if (*(u8 *)&o->field_3a != 0) {
        o->field_07++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        func_801204f4(o, o->side, 8);
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0x1a, 0x33);
    }
}

void func_801b2c74_slot04_12(Object *o) {
    u8 t;

    func_80130efc(o);
    if (*(u8 *)&o->field_3a == 0) {
        o->field_07++;
        if (o->field_4b != 0) {
            t = 3;
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            t = o->field_12a >> 1;
        }
        ((Slot04bObj *)o)->field_27b = data_801c4c74_slot04_12[t];
        o->field_165 = 0;
        func_801204f4(o, o->side, 9);
    }
}

/* The call of func_8011f0e8 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 1 instruction slots. */
void func_801b2d20_slot04_12(Object *obj) {
    Object *p;
    s16 y;

    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        p = ((Object *(*)(Object *))func_8011f0e8)(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x1d;
            p->field_ad = 1;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_26 = obj->field_26;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->field_70 = obj->field_70;
            p->field_3c = obj;
            p->field_7a = obj->field_7a;
            p->field_7c = obj->field_7c;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            func_801204f4(obj, obj->side, 0x11);
        }
        func_801307e0(obj, obj->field_12a + 0x5a);
    }
}
