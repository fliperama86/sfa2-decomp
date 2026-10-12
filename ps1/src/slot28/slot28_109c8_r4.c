/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8004384c_slot28[];
extern SequenceStep *data_80043858_slot28[];
extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
void func_800210f0_slot28(void);
void func_80021060_slot28(Object *obj, int arg);

void advance_slot28_pass_to_step_three_b_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index++;
        func_800210f0_slot28();
        set_sequence_from_table_5(data_80051b40_slot28.p, 2, data_8004384c_slot28);
        p = data_80051b44_slot28[0];
        p->pos_x = 0x48;
        p->pos_y = 0x20;
        set_sequence_from_table_5(p, 1, data_80043858_slot28);
        func_80021060_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
