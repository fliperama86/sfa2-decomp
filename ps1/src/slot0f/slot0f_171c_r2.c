/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int get_usable_memory_card_mask_slot0f(void);
void update_menu_selection_with_enabled_mask_slot0f(Object *o);
void reset_state_index_and_handler_indices_slot0f(Object *o);

void enter_card_slot_select_with_ot_depth_one_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    o->field_01 += 1;
    o->image_row = get_usable_memory_card_mask_slot0f();
    o->ot_depth = 1;
    obj->field_0b = 0;
    update_menu_selection_with_enabled_mask_slot0f(o);
}

void update_menu_select_with_confirm_cancel_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u16 b;
    update_menu_selection_with_enabled_mask_slot0f(o);
    b = o->pos_x_frac;
    if (b & 0x860) {
        if (b & 0x820) {
            if (obj->field_0b == 2) {
                reset_state_index_and_handler_indices_slot0f(o);
            } else {
                obj->field_0c = 0;
                o->field_01 += 1;
                o->state_0 += 1;
            }
            play_sound_at(0, 0, 0x205);
        } else {
            reset_state_index_and_handler_indices_slot0f(o);
            play_sound_at(0, 0, 0x202);
        }
    }
}
