/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051bd0_slot28[];
extern ObjectRef data_80051bd4_slot28;
void func_80022ccc_slot28(void);
void func_80022c3c_slot28(Object *obj, int arg);

void func_80022810_slot28(Object *obj) {
    int t = data_8018f5a0->countdown_timer - 1;
    data_8018f5a0->countdown_timer = t;
    if ((s16)t < 0) {
        set_menu_command_by_code(6, 4);
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}

void advance_pass_clear_objects_and_fade_0_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        func_80022ccc_slot28();
        start_fade_mode_1a01_and_wait();
        data_80051bd0_slot28[0]->field_01 = 0;
        data_80051bd4_slot28.p->field_01 = 0;
        func_80022c3c_slot28(obj, 3);
    }
}
