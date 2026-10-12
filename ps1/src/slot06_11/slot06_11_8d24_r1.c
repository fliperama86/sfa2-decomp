/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801e9da0_slot06_11[];
extern ObjectFn data_801e9db0_slot06_11[];
extern SeqRec *data_801e9d98_slot06_11[];

void set_sequence_by_step_flags_random_timer_slot06_11(Object *obj);

void call_handler_by_state_0_slot06_11(Object *obj) {
    data_801e9da0_slot06_11[obj->state_0](obj);
}

void clear_flag_byte_and_increment_state_0_slot06_11(Object *obj) {
    obj->field_01 = 0;
    obj->state_0++;
}

void call_handler_by_state_1_if_not_paused_slot06_11(Object *obj) {
    if (game_state.field_65 == 0) {
        data_801e9db0_slot06_11[obj->state_1](obj);
    }
}

void start_countdown_sequence_on_random_chance_slot06_11(Object *obj) {
    if ((get_random_byte() & 0x1f) == 5) {
        obj->countdown = (get_random_byte() & 3) + 2;
        obj->step_flags = 1;
        obj->state_1++;
        set_sequence_by_step_flags_random_timer_slot06_11(obj);
    }
}

void toggle_step_flags_and_count_down_slot06_11(Object *obj) {
    obj->step_timer = (s16)obj->step_timer - 1;
    if ((s16)obj->step_timer == 0) {
        u16 t = obj->step_flags ^ 1;

        obj->step_flags = t;
        if (t != 0) {
            obj->countdown = (s16)obj->countdown - 1;
            if ((s16)obj->countdown == 0) {
                obj->state_1++;
            }
        }
        set_sequence_by_step_flags_random_timer_slot06_11(obj);
    }
}

void start_countdown_and_toggle_layer_flags_slot06_11(Object *obj) {
    u16 *p = (u16 *)(data_801aa5d4 + 0x4a);
    u16 *q = (u16 *)(cam_obj + 0x4a);

    obj->state_1++;
    obj->countdown = (u8)get_random_byte();
    *p ^= 0x800;
    *q ^= 0x200;
}

void count_down_then_toggle_layer_flag_bits_slot06_11(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        u16 *p = (u16 *)(data_801aa5d4 + 0x4a);
        u16 *q = (u16 *)(cam_obj + 0x4a);

        obj->state_1 = 0;
        *p ^= 0x800;
        *q ^= 0x200;
        obj->step_flags = 0;
        set_sequence_by_step_flags_random_timer_slot06_11(obj);
    }
}

void release_object_record_slot06_11(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_sequence_by_step_flags_random_timer_slot06_11(Object *obj) {
    set_sequence_from_table_4(obj, data_801e9d98_slot06_11, obj->step_flags);
    obj->step_timer = (get_random_byte() & 3) + 1;
}
