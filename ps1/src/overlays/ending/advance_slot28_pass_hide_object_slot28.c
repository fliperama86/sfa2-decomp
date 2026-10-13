/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051a84_slot28[];
void set_sequence_of_fourth_hud_object_9_slot28(Object *obj, int arg);
void release_active_objects_in_table_7_slot28(void);

void advance_slot28_pass_hide_object_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        screen_state->countdown_timer = 0x258;
        screen_state->pass_index += 1;
        release_active_objects_in_table_7_slot28();
        data_80051a84_slot28[0]->pos_y = -0x40;
        start_fade_mode_1a01_and_wait();
        set_sequence_of_fourth_hud_object_9_slot28(obj, 1);
    }
}
