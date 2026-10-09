/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80011740_slot01(void) {
    Object *p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x30;
        p->field_03 = 0x80;
        if (game_state.mode != 3) {
            if ((player_left.field_f1 | player_right.field_f1) != 0) {
                p->field_03 = 0x82;
            }
        }
    }
}

void func_800117b8_slot01(void) {
    if (game_state.field_ee == 0) {
        game_state.field_f0 = 0x1f;
        ((u16 *)data_80190464)[0] = 0x2201;
        ((u16 *)data_8019046c)[0] = 0x1e00;
        func_80119144(2, 4);
    }
}

void func_8001180c_slot01(void) {
    while (game_state.field_f0 != 0) {
        func_80138164();
        func_801510bc();
        func_8011a784();
        func_801192bc(1);
    }
}
