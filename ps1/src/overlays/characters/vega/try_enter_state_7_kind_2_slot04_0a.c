/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_sixteen_slot_words_slot04_0a(Object *obj);
void set_state_7_and_offset_from_buttons_slot04_0a(Object *obj);

int try_enter_state_7_kind_2_slot04_0a(Object *obj) {
    int r = 0;

    if (obj->field_7e != 0 || obj->link_flags == 0) {
        if (try_action_clearing_both_flags(obj) != 0) {
            r = 1;
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->field_15a = 2;
            clear_sixteen_slot_words_slot04_0a(obj);
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            set_direction_index_from_buttons_group_a(obj);
        }
    }
    return r;
}

int try_enter_state_7_kind_3_offset_0_slot04_0a(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        obj->field_15a = 3;
        ((Slot04aObj *)obj)->offset_index = 0;
        clear_sixteen_slot_words_slot04_0a(obj);
        set_state_7_and_offset_from_buttons_slot04_0a(obj);
    }
    return r;
}

int try_enter_state_7_kind_3_offset_4_slot04_0a(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        obj->field_15a = 3;
        ((Slot04aObj *)obj)->offset_index = 4;
        clear_sixteen_slot_words_slot04_0a(obj);
        set_state_7_and_offset_from_buttons_slot04_0a(obj);
    }
    return r;
}
