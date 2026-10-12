/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051890_slot28;
extern ObjectRef data_800518c8_slot28;
void set_sequence_of_fourth_hud_object_2_slot28(Object *obj, int arg);

void advance_pass_when_object_field_negative_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        p = data_800518c8_slot28.p;
        if (((Slot28Obj *)p)->field_3a < 0) {
            HudState *h = data_8018f5a0;
            h->pass_index = h->pass_index + 1;
            p = data_80051890_slot28.p;
            p->variant_index = 0xff;
            set_sequence_of_fourth_hud_object_2_slot28(obj, 3);
        }
    }
}
