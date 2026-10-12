/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051b00_slot28[];
void set_sequence_of_fourth_hud_object_11_slot28(Object *obj, int arg);

void advance_pass_fade_and_set_sequence_2_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        set_menu_command_by_code(6, 1);
        data_8018f5a0->pass_index++;
        set_sequence_of_fourth_hud_object_11_slot28(obj, 2);
    }
}

void advance_pass_when_object_sequence_ends_slot28(Object *obj) {
    if ((s16)data_80051b00_slot28[4]->step_flags < 0) {
        data_8018f5a0->pass_index++;
        set_menu_command_by_code(1, 0x400);
        start_fade_mode_1801_and_wait();
    }
}
