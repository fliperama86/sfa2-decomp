/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051b48_slot28;
extern ObjectRef data_80051b50_slot28;
void toggle_marker_x_by_input_mask_with_delay_slot28(Object *obj);

void finish_pass_when_slot28_flag_negative_slot28(Object *obj) {
    Object *p;
    toggle_marker_x_by_input_mask_with_delay_slot28(obj);
    p = data_80051b50_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        p = data_80051b48_slot28.p;
        p->pos_x = -0x148;
        data_8018f5a0->countdown_timer = 0x3c;
        data_8018f5a0->pass_index++;
    }
}
