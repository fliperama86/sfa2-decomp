/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c1f80_slot04_07[];

void start_move_with_countdown_and_sound_slot04_07(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    func_80145d20(obj);
    obj->countdown = 0x400;
    play_sound_at(obj, obj->side, 0x31c);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x49);
}

void count_down_then_spawn_pooled_objects_a_slot04_07(Object *obj) {
    int t = obj->countdown - 0x100;

    obj->countdown = t;
    if ((t & 0xff00) == 0) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 5);
        obj->countdown |= 0x3800;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        spawn_children_per_direction_index(obj, -0x28, 0x3c);
    }
}

void count_down_then_stop_and_set_other_lock_slot04_07(Object *obj) {
    int t = obj->countdown - 0x100;
    int i = 0;

    obj->countdown = t;
    if ((t & 0xff00) == 0) {
        obj->field_165 = 0;
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c1f80_slot04_07[i];
    }
}
