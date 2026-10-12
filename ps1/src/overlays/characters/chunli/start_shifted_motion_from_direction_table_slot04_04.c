/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c4390_slot04_04[];

void start_shifted_motion_from_direction_table_slot04_04(Object *o) {
    o->state_3++;
    o->countdown = ((Slot04bObj *)o)->countdown + 0x400;
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    spawn_three_child_objects_at_object_spawn(o);
    if (o->facing_side != 0) {
        o->pos_x = o->pos_x + 0x18;
    } else {
        o->pos_x = o->pos_x - 0x18;
    }
    o->vel_x = data_801c4390_slot04_04[o->direction_index * 2];
    o->accel_x = data_801c4390_slot04_04[o->direction_index * 2 + 1];
    o->vel_y = data_801c4390_slot04_04[o->direction_index * 2 + 2];
    o->accel_y = data_801c4390_slot04_04[o->direction_index * 2 + 3];
    set_sequence_by_side_and_mode(o, o->direction_index + 0x53);
}
