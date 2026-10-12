/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800279d4_slot28(Object *o);

void func_800278b4_slot28(int *p) {
    int t = data_8018f5a0->countdown_timer - 1;
    data_8018f5a0->countdown_timer = t;
    if ((s16)t < 0) {
        data_8018f5a0->step_index_2++;
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
    if (game_state.wait_flag == 0) {
        data_80190568 = 0;
        data_8018f5a0->step_index_2 = 0;
        data_8018f5a0->pass_index = 0;
        data_8018f5a0->substep_index++;
    }
}

void func_8002799c_slot28(int *p) {
    int i;
    for (i = 0; i < 16; i++) {
        Slot28Rec51cf0 *c = (Slot28Rec51cf0 *)(p + 3) + i;
        c->field_00 = 0;
        c->init_value_b = 8;
        c->init_value_a = 8;
        c->field_0a = 0;
    }
}
