/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6288_slot04_14[];

/*
 * What it does (inferred, not an original name): a state entry of a
 * character. It sets active_flag_4 and state_3 to 1, clears substate_index to
 * step_limit, spends 9 points (add_to_meter_clamped), calls add_one_to_ec4_if_object_unflagged with the
 * game state, copies four consecutive words of a table, selected by
 * direction_index, into vel_x, vel_y, accel_x and accel_y, and starts
 * sequence 0x1b (0x61 when field_49 is not 0) plus direction_index / 2.
 */
void func_801b1a00_slot04_14(Object *obj) {
    /* The index is a u8 formed in two statements: with int idx, nothing else changed, this function differs from the original in 51 instruction slots; with idx = obj->direction_index * 2 in one statement inside the wrapper, also 51. */
    u8 idx;
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    obj->step_limit = 0;
    obj->step_count_2 = 0;
    obj->field_12d = 0;
    obj->substate_index = 0;
    add_to_meter_clamped(obj, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 51 instruction slots. */
    /* Inferred reading: the register pass gives registers by uses over life, and the index value competes for v0 with the temporaries of the first three table reads; it wins only with the heavier count, and the 51 slots are a pure exchange of v0 and v1. */
    do {
        idx = obj->direction_index;
        idx <<= 1;
    } while (0);
    obj->vel_x = data_801c6288_slot04_14[idx];
    obj->vel_y = data_801c6288_slot04_14[(u8)(idx + 1)];
    obj->accel_x = data_801c6288_slot04_14[(u8)(idx + 2)];
    obj->accel_y = data_801c6288_slot04_14[(u8)(idx + 3)];
    a = 0x1b;
    if (obj->field_49 != 0) {
        a = 0x61;
    }
    a += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, a);
}
