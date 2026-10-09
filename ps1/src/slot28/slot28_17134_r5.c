/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_800279d4_slot28(Object *o);

void func_800278b4_slot28(int *p) {
    int t = data_8018f5a0->field_60 - 1;
    data_8018f5a0->field_60 = t;
    if ((s16)t < 0) {
        data_8018f5a0->field_50++;
        func_801280f0();
    }
    func_800279d4_slot28((Object *)p);
    func_80138164();
    func_8011abe4();
    func_8014f4d4(6, 2);
}

void func_80027930_slot28(int *p) {
    func_800279d4_slot28((Object *)p);
    func_80138164();
    func_8011abe4();
    if (game_state.field_f0 == 0) {
        data_80190568 = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_52 = 0;
        data_8018f5a0->field_4e++;
    }
}

void func_8002799c_slot28(int *p) {
    int i;
    for (i = 0; i < 16; i++) {
        Slot28Rec51cf0 *c = (Slot28Rec51cf0 *)(p + 3) + i;
        c->field_00 = 0;
        c->field_09 = 8;
        c->field_08 = 8;
        c->field_0a = 0;
    }
}
