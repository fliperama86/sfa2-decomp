/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_move_with_direction_interrupt_slot04_07(Object *obj) {
    s16 t = obj->step_flags;

    if ((t & 0x8000) != 0) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0x80) != 0 && match_pressed_direction_input(obj) != 0) {
            goto reset;
        } else if (((Slot04aObj *)obj)->field_1ca == 7 && obj->direction_index == 4 && (obj->input_bits_2 & 0x4000) == 0 && (obj->pressed_bits & 8) != 0) {
            obj->state_3 = 2;
            obj->input_dir_second = 2;
            reset_hold_flags_and_other_hit_count(obj);
            if (obj->facing_side == 0) {
                obj->pos_x -= 0x20;
            } else {
                obj->pos_x += 0x20;
            }
            reset_direction_and_action_fields(obj);
            set_sequence_by_side_and_mode(obj, 0x2f);
            return;
        } else if ((u8)try_set_direction_from_pressed_bits(obj) == 0) {
            update_sequence(obj);
        } else {
            ((Slot04aObj *)obj)->field_1ca |= 1 << (obj->direction_index >> 1);
            if (obj->facing_side == 0) {
                obj->pos_x -= 0x18;
            } else {
                obj->pos_x += 0x18;
            }
        reset:
            obj->field_25f = 0xff;
            obj->state_3 = 0;
            update_sequence(obj);
        }
    }
}
