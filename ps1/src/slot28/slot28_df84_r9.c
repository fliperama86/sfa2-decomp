/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051ad0_slot28;
extern SequenceStep *data_8003f0b8_slot28[];
extern Object *data_80051ac4_slot28[];
extern u8 data_8003dea0_slot28[];
extern u8 data_8003e230_slot28[];
extern Object *data_80051a98_slot28[];

void func_8001ed94_slot28(Object *obj) {
    if ((s16)data_80051ad0_slot28.p->step_flags < 0) {
        data_8018f5a0->countdown_timer = 0xb4;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
    }
}

void func_8001edd4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        set_menu_command_by_code(6, 3);
        start_fade_mode_1_and_wait();
        data_8018f5a0->pass_index += 1;
    }
}

void func_8001ee3c_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index += 1;
    }
}

void func_8001ee74_slot28(Object *obj, int arg) {
    set_sequence_from_table_5(data_80051ac4_slot28[3], arg, data_8003f0b8_slot28);
}

void func_8001eea4_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_8003dea0_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_8003e230_slot28;
}

void func_8001eee0_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051a98_slot28[i] = 0;
    }
}
