/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051964_slot28;
extern Object *data_80051954_slot28[];
extern ObjectRef data_80051958_slot28;
extern ObjectRef data_8005195c_slot28;
extern u8 data_80032a64_slot28[];
extern u8 data_80032ec8_slot28[];
extern SequenceStep *data_80034530_slot28[];
void func_80019190_slot28(void);
void func_80019100_slot28(Object *obj, int arg);

void func_8001875c_slot28(Object *obj) {
    Object *b = data_80051964_slot28.p;
    if (b->field_6b != 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 += 1;
        func_80019190_slot28();
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            int one = 1;
            b->field_00 = one;
            b->field_02 = 0x20;
            b->field_03 = 8;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80032a64_slot28;
            b->field_9c = data_80032ec8_slot28;
            b->pos_x = 0x118;
            b->pos_y = 0xa0;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_01 = one;
            b->field_0d = 0;
            b->field_09 = 2;
            func_80130768(b, 4, data_80034530_slot28);
            data_80051954_slot28[0] = b;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            int one = 1;
            b->field_00 = one;
            b->field_02 = 0xa5;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80032a64_slot28;
            b->field_9c = data_80032ec8_slot28;
            b->pos_x = 0xc0;
            b->pos_y = 0x60;
            b->field_7a = 0x60;
            b->field_7c = 0x1e1;
            b->field_03 = 0;
            b->field_01 = one;
            b->field_0d = 0;
            b->field_09 = 4;
            func_80130768(b, 0xa, data_80034530_slot28);
            data_80051958_slot28.p = b;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            int one = 1;
            b->field_00 = one;
            b->field_02 = 0xa5;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80032a64_slot28;
            b->field_9c = data_80032ec8_slot28;
            b->pos_x = 0xc0;
            b->pos_y = 0xa0;
            b->field_7a = 0x60;
            b->field_7c = 0x1e1;
            b->field_03 = 0;
            b->field_01 = one;
            b->field_0d = 0;
            b->field_09 = 4;
            func_80130768(b, 0xa, data_80034530_slot28);
            data_8005195c_slot28.p = b;
        }
        func_80128370();
        func_80019100_slot28(obj, 2);
    }
}
