/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
extern ObjectRef data_80051b48_slot28;
extern ObjectRef data_80051b50_slot28;
extern ObjectRef data_80051b54_slot28;
extern u8 data_80051b58_slot28;
extern u8 data_80051b5c_slot28;
extern u8 data_80051b60_slot28;
extern SequenceStep *data_8004384c_slot28[];
extern SequenceStep *data_80043848_slot28[];
extern SequenceStep *data_80043858_slot28[];
extern SequenceStep *data_80043860_slot28[];
extern SequenceStep *data_80043870_slot28[];
void func_800210cc_slot28(void);
void func_80021090_slot28(Object *obj);

void func_800206d4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->field_60 = 0xb4;
    h->field_50++;
    func_80151020(0x306);
    func_800210cc_slot28();
    func_801280f0();
    data_80190568 = one;
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021090_slot28(b);
        data_80051b40_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->field_7a = 0;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = one;
        func_80130768(b, 1, data_8004384c_slot28);
        b->field_01 = one;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021090_slot28(b);
        data_80051b54_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->field_7a = 0;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = one;
        func_80130768(b, 0, data_80043848_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021090_slot28(b);
        data_80051b44_slot28[0] = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->field_7a = 0x10;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 5;
        func_80130768(b, 0, data_80043858_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021090_slot28(b);
        data_80051b48_slot28.p = b;
        b->pos_x = 0x58;
        b->pos_y = -0x60;
        b->field_7a = 0x20;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 4;
        func_80130768(b, 0, data_80043860_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021090_slot28(b);
        data_80051b50_slot28.p = b;
        b->field_7c = 0x1e0;
        b->pos_x = 0x28;
        b->field_7a = 0;
        b->field_0d = 0;
        b->field_03 = one;
        b->field_50 = 0xb8;
        b->field_09 = 0;
        func_80130768(b, 0, data_80043870_slot28);
    }
    data_80051b58_slot28 = 5;
    data_80051b5c_slot28 = 0x20;
    data_80051b60_slot28 = 0;
    func_8012818c();
}
