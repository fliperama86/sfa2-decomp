/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1598_slot04_05[];
extern u8 data_801c1590_slot04_05[];

void start_move_from_direction_tables_slot04_05(Object *o) {
    o->field_225 = 1;
    o->state_3++;
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    spawn_three_child_objects_at_object_spawn(o);
    o->vel_x = data_801c1598_slot04_05[o->direction_index];
    o->accel_x = data_801c1598_slot04_05[o->direction_index + 1];
    set_sequence_by_side_and_mode(o, *(s16 *)((u8 *)data_801c1590_slot04_05 + (o->direction_index & 0xfe)));
}

void spawn_children_on_step_flag_then_update_sequence_slot04_05(Object *o) {
    if ((u8)o->step_flags != 0) {
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        o->state_3++;
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0x10, 0x43);
    }
    update_sequence(o);
}
