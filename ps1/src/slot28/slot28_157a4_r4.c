/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051c9c_slot28;
extern SequenceStep *data_8004dd0c_slot28[];
extern Object *data_80051c90_slot28[];
extern u8 data_8004c73c_slot28[];
extern u8 data_8004cbc4_slot28[];
extern Object *data_80051c64_slot28[];

void func_80025d4c_slot28(Object *obj) {
    if ((s16)data_80051c9c_slot28.p->step_flags < 0) {
        data_8018f5a0->countdown_timer = 0xb4;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
    }
}

void func_80025d8c_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        func_8014f4d4(6, 3);
        func_801280f0();
        data_8018f5a0->pass_index += 1;
    }
}

void func_80025df4_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index += 1;
    }
}

void func_80025e2c_slot28(Object *obj, int arg) {
    func_80130768(data_80051c90_slot28[3], arg, data_8004dd0c_slot28);
}

void func_80025e5c_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_8004c73c_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_8004cbc4_slot28;
}

void func_80025e98_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051c64_slot28[i] = 0;
    }
}
