/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051a44_slot28[];
void set_sequence_of_fourth_hud_object_8_slot28(Object *obj, int arg);
void release_active_objects_in_table_6_slot28(void);

void advance_slot28_scene_hide_object_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        screen_state->pass_index += 1;
        release_active_objects_in_table_6_slot28();
        set_sequence_of_fourth_hud_object_8_slot28(obj, 2);
        data_80051a44_slot28[0]->field_01 = 0;
        start_fade_mode_1a01_and_wait();
    }
}
