/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_input_sequence_attack_with_gauge_drain_slot04_07(Object *obj) {
    int k;

    if (obj->mode_flag_2 == 0) {
        switch (((Slot04aObj *)obj)->field_1c8) {
        case 0:
            if (obj->input_bits_2 == 0x8000) {
                ((Slot04aObj *)obj)->field_1c9 = 4;
                ((Slot04aObj *)obj)->field_1c8 = ((Slot04aObj *)obj)->field_1c8 + 1;
            }
            break;
        case 1:
            if (obj->input_bits_2 == 0x1000) {
                ((Slot04aObj *)obj)->field_1c9 = 4;
                ((Slot04aObj *)obj)->field_1c8 = ((Slot04aObj *)obj)->field_1c8 + 1;
            } else {
                ((Slot04aObj *)obj)->field_1c9 = ((Slot04aObj *)obj)->field_1c9 - 1;
                if ((((Slot04aObj *)obj)->field_1c9 & 0x80) != 0) {
                    ((Slot04aObj *)obj)->field_1c8 = 0;
                }
            }
            break;
        case 2:
            if (obj->input_bits_2 == 0x2000) {
                ((Slot04aObj *)obj)->field_1c9 = 4;
                ((Slot04aObj *)obj)->field_1c8 = ((Slot04aObj *)obj)->field_1c8 + 1;
            } else {
                ((Slot04aObj *)obj)->field_1c9 = ((Slot04aObj *)obj)->field_1c9 - 1;
                if ((((Slot04aObj *)obj)->field_1c9 & 0x80) != 0) {
                    ((Slot04aObj *)obj)->field_1c8 = 0;
                }
            }
            break;
        case 3:
            if ((obj->pressed_bits & 0x14) != 0) {
                obj->state_3 = obj->state_3 + 1;
                set_sequence_by_side_and_mode(obj, 0x18);
                return;
            }
            ((Slot04aObj *)obj)->field_1c9 = ((Slot04aObj *)obj)->field_1c9 - 1;
            if ((((Slot04aObj *)obj)->field_1c9 & 0x80) != 0) {
                ((Slot04aObj *)obj)->field_1c8 = 0;
            }
            break;
        }
    }
    if (*(u8 *)&obj->step_flags != 0) {
        spawn_effect_at_object_offset(obj, 1, -0x1a, 0x3a);
        play_sound_at(obj, obj->side ^ 1, 0x30b);
        play_side_sound_at(obj, obj->side, 9);
        k = 0;
        obj->step_flags = obj->step_flags & 0xff00;
        if (((Slot04aObj *)obj)->field_1ce == 0) {
            ((Slot04aObj *)obj)->field_1ce = 1;
            k = 7;
        }
        if ((u8)apply_damage_to_other_return_defeated(obj, (s16)k, 0) != 0) {
            obj->state_3 = obj->state_3 + 3;
            set_sequence_by_side_and_mode(obj, 0x1a);
            return;
        }
        obj->countdown = obj->countdown + 0x100;
    }
    k = obj->pressed_bits;
    if (k != 0) {
        if (*(s16 *)&obj->step_timer > 1) {
            obj->step_timer = *(s16 *)&obj->step_timer - 1;
        }
    }
    if ((*(s16 *)&obj->countdown & 0xff00) == 0 || (obj->other->field_15b != 0 && (u8)drain_gauge_by_other_input(obj) == 0)) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 3;
        set_sequence_by_side_and_mode(obj, 0x1a);
    }
}
