/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea998_slot06_01[];
extern SequenceStep *data_801eb984_slot06_01[];
extern SequenceStep *data_801eb9ac_slot06_01;

void switch_sequence_on_player_state_2_slot06_01(Object *obj) {
    if (player_left.state_2 == 5 || player_right.state_2 == 8) {
        obj->state_1++;
        set_sequence_entry(obj, data_801eb9ac_slot06_01);
    } else {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_1 = 1;
        }
        advance_sequence_checked(obj);
    }
}

void set_state_1_to_one_at_sequence_end_slot06_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 1;
    }
    advance_sequence_checked(obj);
}

void release_object_record_2_slot06_01(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_01(Object *obj) {
    data_801ea998_slot06_01[obj->state_0](obj);
}

void init_object_spawn_and_flip_pos_y_2_slot06_01(Object *obj) {
    SequenceStep *s;
    u16 y;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->base_x = 0x300;
    obj->base_y = 0x100;
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1e0;
    obj->state_0++;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    y = obj->pos_y;
    obj->pos_y = 0xf8 - y;
    set_sequence_entry(obj, data_801eb984_slot06_01[0]);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_sequence_unless_paused_then_show_slot06_01(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        advance_sequence_checked(obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void release_object_record_3_slot06_01(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
