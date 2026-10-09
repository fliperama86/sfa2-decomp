/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_8003a1a0_slot28[];
extern u8 data_8003a5d4_slot28[];
extern SequenceStep *data_8003b504_slot28[];
extern SequenceStep *data_8003b510_slot28[];
extern SequenceStep *data_8003b514_slot28[];
extern Object *data_80051a18_slot28[];
extern ObjectRef data_80051a40_slot28;
extern Object *data_80051a44_slot28[];
extern ObjectRef data_80051a48_slot28;
void func_8001d548_slot28(Object *obj, int idx);
void func_8001d578_slot28(Object *obj);
void func_8001d5d8_slot28(void);

void func_8001d184_slot28(Object *obj) {
    Object *o;
    int i;
    Object **slot;
    int one;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_8001d5d8_slot28();
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            func_8001d578_slot28(o);
            data_80051a48_slot28.p = o;
            o->pos_x = 0x60;
            o->pos_y = -0x60;
            o->field_7a = 0x20;
            o->field_7c = 0x1e0;
            o->field_0d = 0;
            o->field_09 = 5;
            func_80130768(o, 0, data_8003b510_slot28);
            o->field_01 = 1;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = 1;
            o->field_01 = 1;
            o->field_02 = 0x2e;
            o->field_90 = (void *)0x80060000;
            o->field_98 = data_8003a1a0_slot28;
            o->field_9c = data_8003a5d4_slot28;
            o->pos_x = 0x18;
            o->pos_y = 0x90;
            o->field_7a = 0x60;
            o->field_7c = 0x1e0;
            o->field_09 = 3;
            o->field_03 = 0;
            o->field_0d = 0;
            data_80051a18_slot28[0] = o;
            o->box_tables = (BoxTables *)data_8003b514_slot28;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = 1;
            o->field_02 = 0x2e;
            o->field_90 = (void *)0x80060000;
            o->field_98 = data_8003a1a0_slot28;
            o->field_9c = data_8003a5d4_slot28;
            o->pos_x = 0x150;
            o->pos_y = 0x90;
            o->field_7a = 0x60;
            o->field_7c = 0x1e0;
            o->field_09 = 3;
            o->field_01 = 1;
            o->field_03 = 1;
            o->field_0d = 0;
            data_80051a18_slot28[1] = o;
            o->box_tables = (BoxTables *)data_8003b514_slot28;
        }
        i = 0;
        one = 1;
        slot = &data_80051a18_slot28[2];
        for (; i < 6; i++) {
            o = (Object *)func_8011f1e0();
            if (o != 0) {
                o->field_02 = 0x2e;
                o->field_03 = 3;
                o->field_90 = (void *)0x80060000;
                o->field_98 = data_8003a1a0_slot28;
                o->field_9c = data_8003a5d4_slot28;
                o->field_7a = 0x60;
                o->field_7c = 0x1e0;
                o->field_09 = 4;
                o->field_00 = one;
                o->field_01 = one;
                o->field_0d = 0;
                *slot = o;
                o->box_tables = (BoxTables *)data_8003b514_slot28;
            }
            slot++;
        }
        func_80130768(data_80051a40_slot28.p, 0, data_8003b504_slot28);
        o = data_80051a44_slot28[0];
        o->field_01 = 0;
        o = data_80051a48_slot28.p;
        o->field_01 = 1;
        o->pos_x = 0x58;
        o->pos_y = 0x10;
        o->field_09 = 5;
        func_8001d548_slot28(obj, 4);
        func_80128370();
    }
}
