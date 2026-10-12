/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_800212ac_slot01[];
extern void (*data_80015320_slot01[])(Object *);
void spawn_linked_object_by_variant_slot01(Object *obj);
void advance_record_list_and_set_spawn_y_slot01(Object *obj);

void set_sequence_from_slot01_table_slot01(Object *obj, u8 a) {
    set_sequence_from_table_5(obj, a, data_800212ac_slot01);
}

void update_object_by_state_1_table_slot01(Object *obj) {
    if (!(obj->table_index & 0x80)) {
        if (obj->table_index == 0) {
            if (game_state.status_flags != 0) {
                obj->state_1 = 3;
            }
            data_80015320_slot01[obj->state_1](obj);
            advance_record_list_and_set_spawn_y_slot01(obj);
        }
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void count_down_spawn_linked_object_and_add_vel_x_slot01(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t < 0) {
        obj->countdown = 1;
        obj->state_1++;
        spawn_linked_object_by_variant_slot01(obj);
    }
    obj->pos_x = obj->pos_x + obj->vel_x;
}
