/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_vel_x_from_direction_table_slot04_06(Object *obj);

void start_grab_attack_with_countdown_2_slot04_06(Object *o) {
    ((Slot04aObj *)o)->countdown_2 = 0x32;
    o->state_3++;
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    spawn_three_child_objects_at_object_spawn(o);
    play_side_sound_at(o, o->side, 7);
    o->substate_index = 0;
    o->field_12d = 0;
    o->step_count_2 = 0;
    o->step_limit = 0;
    o->accel_x = -0x6000;
    set_vel_x_from_direction_table_slot04_06(o);
    set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x34);
}
