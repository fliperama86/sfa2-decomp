/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051a98_slot28[];
extern ObjectRef data_80051ac0_slot28;
extern Object *data_80051ac4_slot28[];
extern SequenceStep *data_8003f084_slot28[];
extern SequenceStep *data_8003f090_slot28[];
extern SequenceStep *data_8003f09c_slot28[];
void func_8001ef04_slot28(void);
void func_8001eea4_slot28(Object *obj);
void func_8001ee74_slot28(Object *obj, int arg);

void func_8001ea3c_slot28(Object *obj) {
    Object *b;
    if (data_80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52++;
        func_8001ef04_slot28();
        func_80130768(data_80051ac0_slot28.p, 2, data_8003f084_slot28);
        b = data_80051ac4_slot28[0];
        b->pos_x = 0x78;
        b->pos_y = 0x20;
        b->field_01 = 1;
        func_80130768(b, 1, data_8003f090_slot28);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            func_8001eea4_slot28(b);
            b->pos_x = 0xb0;
            b->pos_y = 0xa0;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_09 = 3;
            func_80130768(b, 6, data_8003f09c_slot28);
            data_80051a98_slot28[0] = b;
        }
        func_8001ee74_slot28(obj, 2);
        func_80128370();
    }
}
