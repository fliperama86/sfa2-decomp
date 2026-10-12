/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800e87a8_slot0f[])(Object *);
int get_usable_memory_card_mask_slot0f(void);
void update_menu_selection_with_enabled_mask_slot0f(Object *o);

void update_countdown_reset_on_button_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    if (o->pos_x_frac & 0x860) {
        obj->countdown = 1;
    }
    obj->countdown -= 1;
    if (obj->countdown == 0) {
        o->field_01 = 0;
        o->state_0 = 0;
        if (obj->mode == 4) {
            o->active_count = 4;
            o->ot_depth = 3;
        }
    }
}

void func_800e1af4_slot0f(Object *o) {
    data_800e87a8_slot0f[(s8)o->field_01](o);
}

void enter_card_slot_select_with_ot_depth_two_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    o->field_01 += 1;
    o->image_row = get_usable_memory_card_mask_slot0f();
    o->ot_depth = 2;
    obj->field_0b = 0;
    update_menu_selection_with_enabled_mask_slot0f(o);
}
