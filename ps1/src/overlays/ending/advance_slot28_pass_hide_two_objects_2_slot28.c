/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
void release_active_objects_in_table_15_slot28(void);
void set_sequence_of_fourth_hud_object_17_slot28(Object *obj, int arg);

void advance_slot28_pass_hide_two_objects_2_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->pass_index++;
        release_active_objects_in_table_15_slot28();
        data_80051c90_slot28[0]->field_01 = 0;
        data_80051c94_slot28.p->field_01 = 0;
        set_sequence_of_fourth_hud_object_17_slot28(obj, 1);
        start_fade_mode_1a01_and_wait();
    }
}
