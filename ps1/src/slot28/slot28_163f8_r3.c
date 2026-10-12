/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80050304_slot28[];
extern Object *data_80051cd0_slot28[];
extern u8 data_8004ead4_slot28[];
extern u8 data_8004ef00_slot28[];
extern Object *data_80051ca4_slot28[];

void func_800271bc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        func_8014f4d4(6, 3);
        func_801280f0();
        data_8018f5a0->pass_index += 1;
    }
}

void func_80027224_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index += 1;
    }
}

void func_8002725c_slot28(Object *obj, int arg) {
    func_80130768(data_80051cd0_slot28[3], arg, data_80050304_slot28);
}

void func_8002728c_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_8004ead4_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_8004ef00_slot28;
}

void func_800272c8_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051ca4_slot28[i] = 0;
    }
}

void func_800272ec_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051ca4_slot28[i];
        if (p != 0 && p->active_count != 0) {
            func_8011f240((Slab172 *)p);
            data_80051ca4_slot28[i] = 0;
        }
    }
}
