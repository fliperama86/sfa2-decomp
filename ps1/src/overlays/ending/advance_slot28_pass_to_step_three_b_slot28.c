/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_8004384c_slot28[];
extern SequenceStep *data_80043858_slot28[];
extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
void release_active_objects_in_table_10_slot28(void);
void set_sequence_of_fourth_hud_object_12_slot28(Object *obj, int arg);

void advance_slot28_pass_to_step_three_b_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        screen_state->countdown_timer = 0x258;
        screen_state->pass_index++;
        release_active_objects_in_table_10_slot28();
        set_sequence_from_table_5(data_80051b40_slot28.p, 2, data_8004384c_slot28);
        p = data_80051b44_slot28[0];
        p->pos_x = 0x48;
        p->pos_y = 0x20;
        set_sequence_from_table_5(p, 1, data_80043858_slot28);
        set_sequence_of_fourth_hud_object_12_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
