/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_8005198c_slot28;
extern ObjectRef data_80051958_slot28;

void scroll_two_objects_x_with_wrap_slot28(Object *obj);
void set_sequence_of_fourth_hud_object_5_slot28(Object *obj, int arg);
void scroll_then_advance_pass_to_sequence_4_slot28(Object *obj) {
    Object *p;
    scroll_two_objects_x_with_wrap_slot28(obj);
    p = data_8005198c_slot28.p;
    if ((s16)p->step_flags < 0) {
        HudState *h = screen_state;
        h->countdown_timer = 0xb4;
        h->pass_index += 1;
        p = data_80051958_slot28.p;
        p->variant_index = 0xff;
        set_sequence_of_fourth_hud_object_5_slot28(obj, 4);
    }
}
