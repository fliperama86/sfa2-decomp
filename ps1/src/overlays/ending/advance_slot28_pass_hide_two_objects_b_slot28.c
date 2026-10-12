/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_8004384c_slot28[];
extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
extern ObjectRef data_80051b48_slot28;
void release_active_objects_in_table_10_slot28(void);
void set_sequence_of_fourth_hud_object_12_slot28(Object *obj, int arg);

void advance_slot28_pass_hide_two_objects_b_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->pass_index++;
        data_80051b44_slot28[0]->field_01 = 0;
        data_80051b48_slot28.p->field_01 = 0;
        release_active_objects_in_table_10_slot28();
        set_sequence_from_table_5(data_80051b40_slot28.p, 0, data_8004384c_slot28);
        set_sequence_of_fourth_hud_object_12_slot28(obj, 1);
        start_fade_mode_1a01_and_wait();
    }
}
