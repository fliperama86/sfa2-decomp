/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_8005198c_slot28;
extern ObjectRef data_80051964_slot28;
extern ObjectRef data_80051968_slot28;
extern u8 data_80032a64_slot28[];
extern u8 data_80032ec8_slot28[];
extern SequenceStep *data_80034530_slot28[];

void func_800185d0_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    Object *b = data_8005198c_slot28.p;
    if ((s16)b->field_3a < 0) {
        data_8018f5a0->field_52++;
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x20;
            b->field_03 = 2;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80032a64_slot28;
            b->field_9c = data_80032ec8_slot28;
            b->pos_x = 0xc0;
            b->field_7a = 0x60;
            b->field_01 = 1;
            b->pos_y = 0;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_09 = 2;
            b->field_6b = 0;
            func_80130768(b, 0xa, data_80034530_slot28);
            data_80051964_slot28.p = b;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x20;
            b->field_03 = 3;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80032a64_slot28;
            b->field_9c = data_80032ec8_slot28;
            b->pos_x = 0xc0;
            b->pos_y = 0x100;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_01 = 1;
            b->field_0d = 0;
            b->field_09 = 2;
            func_80130768(b, 0xa, data_80034530_slot28);
            data_80051968_slot28.p = b;
        }
        func_80120554(0, obj->field_227, 0x300);
    }
}
