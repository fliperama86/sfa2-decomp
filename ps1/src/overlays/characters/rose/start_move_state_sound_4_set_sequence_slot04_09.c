/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c7c44_slot04_09[];
void spawn_linked_pooled_object_health_0_slot04_09(Object *obj);

void start_move_state_sound_4_set_sequence_slot04_09(Object *obj) {
    int n = 0x25;

    obj->active_flag_4 = 1;
    obj->field_225 = 1;
    obj->state_3++;
    if (obj->counter == 0) {
        obj->field_225 = 0;
        add_to_meter_clamped(obj, 4);
    }
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_side_sound_at(obj, obj->side, 4);
    if (obj->field_49 != 0) {
        n = 0x43;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + n);
}

void spawn_linked_object_on_step_flag_slot04_09(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        spawn_linked_pooled_object_health_0_slot04_09(obj);
        obj->countdown = data_801c7c44_slot04_09[obj->direction_index >> 1];
    }
    update_sequence(obj);
}

void update_countdown_or_finish_sequence_slot04_09(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags & 0x8000) {
        obj->variant_index_2 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t != 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        start_action_if_buttons_pressed(obj);
    tail:
        update_sequence(obj);
    }
}
