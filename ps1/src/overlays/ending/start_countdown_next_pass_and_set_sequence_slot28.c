/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051be4_slot28[];
extern ObjectRef data_80051c1c_slot28;
void set_sequence_of_fourth_hud_object_15_slot28(Object *obj, int arg);

void start_countdown_next_pass_and_set_sequence_slot28(Object *obj) {
    Object *p;
    p = data_80051c1c_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        screen_state->countdown_timer = 0x12c;
        screen_state->pass_index++;
        p = data_80051be4_slot28[3];
        p->variant_index = 0xff;
        set_sequence_of_fourth_hud_object_15_slot28(obj, 1);
    }
}
