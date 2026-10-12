/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_sixteen_slot_words_slot04_0a(Object *obj);
void set_state_7_and_offset_from_buttons_slot04_0a(Object *obj) {
    int k;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->direction_index = 0;
    k = ((obj->pressed_bits | obj->released_bits) & 0x6a) != 0;
    ((Slot04aObj *)obj)->offset_index += k << 1;
}

int try_start_meter_state_8_move_4_slot04_0a(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        clear_sixteen_slot_words_slot04_0a(obj);
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
    }
    return r;
}

int try_start_meter_state_8_move_5_slot04_0a(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        clear_sixteen_slot_words_slot04_0a(obj);
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}
