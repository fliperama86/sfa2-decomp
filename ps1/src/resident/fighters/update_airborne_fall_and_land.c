/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_airborne_fall_and_land(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object)) {
        store_image_row_for_hit_type(object);
        if (object->hit_type == 0x13) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                update_sequence(object);
            } else if (object->vel_y >= 0) {
                update_sequence(object);
            }
        } else if (object->hit_type == 0x11) {
            update_sequence(object);
        } else if (object->vel_y < 0) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                if ((s16)object->health < 0 || object->field_15b != 0) {
                    update_sequence(object);
                } else {
                    object->state_3 = 6;
                    if (object->field_2a3) {
                        object->field_2a3 = 0;
                    }
                    set_sequence_by_side_and_mode_2(object, 0x18);
                }
            }
        }
    } else {
        object->state_3 = object->state_3 + 1;
        if ((s16)object->health < 0) {
            play_kind_dependent_sound_by_a_and_b(object, 0, 1);
        } else {
            play_kind_dependent_sound_by_a_and_b(object, 0, 0);
        }
        object->active_state = 0;
        object->countdown = 4;
        object->pos_y = (u16)object->ground_y;
        push_other_object_x_by_table_2(object);
        set_side_flag(object);
        object->once_flag_2 = 0;
        object->done_flag = 0;
        object->flag_25a_set = 0;
        choose_recovery_sequence_or_state(object);
    }
}

void update_countdown_hit_type_sequence(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v != 0) {
        if (object->hit_type == 0x13) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                update_sequence(object);
            } else if (object->vel_y >= 0) {
                update_sequence(object);
            }
        } else if (object->hit_type == 0x11) {
            update_sequence(object);
        } else if (object->vel_y < 0) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                if ((s16)object->health < 0 || object->field_15b != 0) {
                    update_sequence(object);
                } else {
                    object->state_3 = 6;
                    if (object->field_2a3) {
                        object->field_2a3 = 0;
                    }
                    set_sequence_by_side_and_mode_2(object, 0x18);
                }
            }
        }
    } else {
        object->active_state = 0xff;
        object->state_3 = object->state_3 + 1;
        set_velocity_from_table(object);
        set_sequence_by_side_and_mode_2(object, 0x15);
    }
}
