/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051ba4_slot28[];
extern ObjectRef data_80051bdc_slot28;
void set_sequence_of_fourth_hud_object_14_slot28(Object *obj, int arg);

void next_pass_and_menu_command_when_field_negative_slot28(Object *obj) {
    Object *a;
    a = data_80051bdc_slot28.p;
    if (((Slot28Obj *)a)->field_3a < 0) {
        data_8018f5a0->countdown_timer = 0x1a4;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        a = data_80051ba4_slot28[0];
        a->variant_index = 0xff;
        a = data_80051ba4_slot28[1];
        a->variant_index = 0xff;
        set_menu_command_by_code(6, 3);
        data_8018f5a0->countdown_timer = 0x1e;
    }
}

void advance_pass_when_countdown_negative_slot28(Object *obj) {
    int t = data_8018f5a0->countdown_timer - 1;
    data_8018f5a0->countdown_timer = t;
    if ((s16)t < 0) {
        data_8018f5a0->countdown_timer = 0x1a4;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        set_sequence_of_fourth_hud_object_14_slot28(obj, 1);
    }
}

void menu_command_and_fade_when_field_negative_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051bdc_slot28.p)->field_3a < 0) {
        set_menu_command_by_code(1, 0x505);
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
