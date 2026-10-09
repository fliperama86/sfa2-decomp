/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051b8c_slot28;
extern Object *data_80051b90_slot28[];
extern ObjectRef data_80051b94_slot28;
extern Object *data_80051b64_slot28[];
extern SequenceStep *data_80045c40_slot28[];
extern SequenceStep *data_80045c58_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_80021d64_slot28(Object *obj, int arg);
void func_80021d94_slot28(Object *obj);
void func_80021df4_slot28(void);

void func_800219a8_slot28(Object *obj) {
    Object *b;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_80021df4_slot28();
        func_80130768(data_80051b8c_slot28.p, 2, data_80045c40_slot28);
        b = data_80051b90_slot28[0];
        b->pos_y = -0x60;
        b = data_80051b94_slot28.p;
        b->field_01 = 0;
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            func_80021d94_slot28(b);
            b->pos_x = 0xb8;
            b->pos_y = 0x70;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->field_09 = 3;
            func_80130768(b, 2, data_80045c58_slot28);
            data_80051b64_slot28[0] = b;
        }
        func_80021d64_slot28(obj, 2);
        func_80128370();
    }
}
