/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c62b0_slot04_02[];

/* What it does (inferred): a state entry of a character. It sets active_flag_4 and state_3 to 1, clears substate_index to step_limit, spends 9 points, calls add_one_to_ec4_if_object_unflagged with the game state and the object, copies four consecutive words of a table, selected by direction_index, into vel_x, vel_y, accel_x and accel_y, and starts sequence 0x1b (0x61 when field_49 is not 0) plus direction_index / 2. */
void start_launch_with_four_velocity_table_values_slot04_02(Object *obj) {
    u16 n;
    int idx;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    obj->step_limit = 0;
    obj->step_count_2 = 0;
    obj->field_12d = 0;
    obj->substate_index = 0;
    add_to_meter_clamped(obj, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    /* Stand-in, not recovered source: two nested do-while wrappers around the statement that sets the table index reproduce the original's choice of registers. Without it this function differs from the original in 51 instruction slots. */
    do {
        do {
            idx = obj->direction_index * 2;
        } while (0);
    } while (0);
    obj->vel_x = data_801c62b0_slot04_02[(u8)idx];
    obj->vel_y = data_801c62b0_slot04_02[(u8)(idx | 1)];
    obj->accel_x = data_801c62b0_slot04_02[(u8)(idx + 2)];
    obj->accel_y = data_801c62b0_slot04_02[(u8)(idx + 3)];
    n = 0x1b;
    if (obj->field_49 != 0) {
        n = 0x61;
    }
    n = (obj->direction_index >> 1) + n;
    set_sequence_by_side_and_mode(obj, n);
}
