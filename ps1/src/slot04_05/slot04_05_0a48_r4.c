/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int is_state_2_8_variant_5_grounded_slot04_05(Object *obj);

int enter_state_1_0_7_0_type_2_slot5_slot04_05(Object *obj) {
    int r = 0;
    u8 one;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->active_flag_3 = one;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

int clear_slot_stages_7_set_flag_1_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int r = 0;
    int i;
    u8 *p;
    u8 c;

    p = &o->slots[7].stage;
    if (is_state_2_8_variant_5_grounded_slot04_05(o)) {
        for (c = r, i = 7; i >= 0; i--) {
            *p++ = c;
        }
        r++;
        obj->field_1a5 = 0x18;
        obj->field_1a6 = 1;
    }
    return r;
}

int clear_slot_stages_8_set_flag_2_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int r = 0;
    int i;
    u8 *p;
    u8 c;

    p = &o->slots[8].stage;
    if (is_state_2_8_variant_5_grounded_slot04_05(o)) {
        for (c = r, i = 7; i >= 0; i--) {
            *p++ = c;
        }
        r++;
        obj->field_1a5 = 0x18;
        obj->field_1a6 = 2;
    }
    return r;
}

int is_state_2_8_variant_5_grounded_slot04_05(Object *obj) {
    int r = 0;

    if (obj->state_2 == 8 && obj->field_15a == 5) {
        r = obj->active_state == 0;
    }
    return r;
}
