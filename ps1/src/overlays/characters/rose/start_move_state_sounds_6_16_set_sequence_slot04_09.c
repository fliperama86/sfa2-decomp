/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_move_state_sounds_6_16_set_sequence_slot04_09(Object *obj) {
    int n = 0x1c;

    obj->active_flag_4 = 1;
    obj->field_225 = 1;
    obj->state_3++;
    if (obj->counter == 0) {
        obj->field_225 = 0;
        add_to_meter_clamped(obj, 4);
    }
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_side_sound_at(obj, obj->side, 6);
    play_side_sound_at(obj, obj->side, 0x16);
    if (obj->field_49 != 0) {
        n = 0x40;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + n);
}

void spawn_pooled_object_on_step_flags_slot04_09(Object *obj) {
    Object *p;

    update_sequence(obj);
    if (*(s16 *)&obj->step_flags != 0) {
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x16;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->scroll_factor = obj->scroll_factor;
            *(s32 *)&p->field_a4 = 0x1000700;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
        obj->state_3++;
    }
}
