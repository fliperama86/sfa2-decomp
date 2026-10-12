/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4210_slot04_04[];
void update_direction_hold_sequence_slot04_04(Object *obj);
void update_sequence_until_exit_then_next_state_slot04_04(Object *obj);

void begin_state_3_sequence_by_direction_index_slot04_04(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->input_matched_flag = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->direction_index;
    set_sequence_by_side_and_mode(obj, a);
}

void update_sequence_or_enter_next_state_3_slot04_04(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->countdown = 0xf;
        obj->state_3++;
        if (obj->mode_flag_2 == 0) {
            update_direction_hold_sequence_slot04_04(obj);
        } else {
            obj->countdown = data_801c4210_slot04_04[obj->input_dir_second];
            update_sequence_until_exit_then_next_state_slot04_04(obj);
        }
    }
}
