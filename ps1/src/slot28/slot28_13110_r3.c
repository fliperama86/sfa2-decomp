/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051be4_slot28[];
extern ObjectRef data_80051c0c_slot28;
extern Object *data_80051c10_slot28[];
extern SequenceStep *data_8004a088_slot28[];
extern SequenceStep *data_8004a09c_slot28[];
extern u8 data_80048d70_slot28[];
extern u8 data_800490f4_slot28[];
extern u8 data_8004aa38_slot28[];
void func_80023b0c_slot28(Object *obj);
void func_80023be0_slot28(Object *obj, int arg);
void func_80023c10_slot28(Object *obj);
void func_80023c70_slot28(void);

void func_80023664_slot28(Object *obj) {
    Object *o;
    int i;
    int one;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52++;
        func_80023c70_slot28();
        func_80130768(data_80051c0c_slot28.p, 1, data_8004a088_slot28);
        o = data_80051c10_slot28[0];
        o->pos_x = 0x18;
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            func_80023c10_slot28(o);
            o->pos_x = 0x90;
            o->pos_y = 0xb1;
            o->field_7a = 0x60;
            o->field_7c = 0x1e0;
            o->field_0d = 0;
            o->field_09 = 2;
            func_80130768(o, 3, (SequenceStep **)data_8004a09c_slot28);
            data_80051be4_slot28[0] = o;
            o->field_09 = 3;
        }
        i = 0;
        one = 1;
        for (; i < 4; i++) {
            o = (Object *)func_8011f1e0();
            if (o != 0) {
                o->field_02 = 0x1d;
                o->field_09 = 3;
                o->field_7a = 0x60;
                o->field_7c = 0x1e0;
                o->pos_x = 0x128;
                o->pos_y = 0xb1;
                o->field_90 = (void *)0x80060000;
                o->field_98 = data_80048d70_slot28;
                o->field_9c = data_800490f4_slot28;
                o->field_00 = one;
                o->field_03 = 0;
                o->field_01 = one;
                o->field_0d = 0;
                o->box_tables = (BoxTables *)data_8004a09c_slot28;
                (data_80051be4_slot28 + 1)[i] = o;
                ((u8 *)&o->field_46)[1] = data_8004aa38_slot28[i * 4];
                o->field_a0 = data_8004aa38_slot28[i * 4 + 2];
            }
        }
        func_80023b0c_slot28(obj);
        func_80023be0_slot28(obj, 2);
        func_80128370();
    }
}
