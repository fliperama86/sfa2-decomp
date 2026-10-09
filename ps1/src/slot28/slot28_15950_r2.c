/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051c64_slot28[];
extern ObjectRef data_80051c8c_slot28;
extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
extern SequenceStep *data_8004dcd8_slot28[];
extern SequenceStep *data_8004dce8_slot28[];
void func_80025ebc_slot28(void);
void func_80025e2c_slot28(Object *obj, int arg);
void func_80025e5c_slot28(Object *obj);

void func_80025c38_slot28(Object *obj) {
    HudState *h;
    Object *b;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52++;
        func_80025ebc_slot28();
        func_80130768(data_80051c8c_slot28.p, 1, data_8004dcd8_slot28);
        b = data_80051c90_slot28[0];
        b->pos_x = 0x48;
        b->pos_y = -0x50;
        b->field_01 = 1;
        b = data_80051c94_slot28.p;
        b->field_01 = 0;
        func_80128370();
        func_80025e2c_slot28(obj, 3);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            func_80025e5c_slot28(b);
            b->pos_x = 0xb8;
            b->pos_y = 0xa0;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_09 = 2;
            func_80130768(b, 5, data_8004dce8_slot28);
            data_80051c64_slot28[0] = b;
        }
    }
}
