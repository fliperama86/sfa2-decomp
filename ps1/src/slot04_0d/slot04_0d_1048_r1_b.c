/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80130678(Object *object, u16 arg);
void func_801b1540_slot04_0d(Object *obj);

extern ObjectFn data_801c2a18_slot04_0d[];
extern u8 data_801c2770_slot04_0d[];

void func_801b1540_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;
    int a = 0x2b;

    o->field_07++;
    t = data_801c2770_slot04_0d[obj->field_1cf];
    o->field_225 = 1;
    o->field_12a = t * 2;
    if (o->field_49 != 0) {
        a = 0x49;
    }
    a += t;
    func_801307e0(o, (u8)a);
}

void func_801b15a4_slot04_0d(Object *obj) {
    Object *p;
    s16 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        ((Slot04bObj *)obj)->field_46 = 5;
        obj->field_07++;
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xd;
            p->field_03 = 0;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->field_5c = 0;
            p->pos_y = y;
            p->field_3c = obj;
            obj->field_14c = (s32)p;
            obj->field_240++;
            if (obj->field_12a == 0) {
                func_801204f4(obj, obj->side, 0x13);
            } else {
                func_801204f4(obj, obj->side, 0x14);
            }
        }
    }
}

void func_801b16f8_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->field_3a < 0) {
        func_801312b8(o);
    } else {
        if (obj->field_46 != 0) {
            obj->field_46--;
            if (obj->field_46 != 0) {
                goto tail;
            }
            o->field_17b = 0;
        }
        func_80142adc(o);
    tail:
        func_80130efc(o);
    }
}

void func_801b1780_slot04_0d(Object *obj) {
    data_801c2a18_slot04_0d[obj->field_07](obj);
}

void func_801b17c0_slot04_0d(Object *obj) {
    int a;

    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    a = 0x2f;
    if (obj->field_45 == 0) {
        a = 0x2b;
    }
    func_80130678(obj, a);
}
