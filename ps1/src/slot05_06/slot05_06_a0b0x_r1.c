/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801dd394_slot05_06[];
extern s32 data_801dd398_slot05_06[];

/* What it does (inferred): enters a character state. It clears the bytes
   substate_index to step_limit, sets active_flag_4 to 1, adds 1 to state_3, calls
   func_80141f28 (object, 7) and func_80138ae8 (&game_state, object), sets
   field_225 to 1, loads vel_x and accel_x from two word tables indexed
   by the byte direction_index (they lie one word apart), and starts sequence
   (direction_index >> 1) + 0x1b. */
void func_801ca0b0_slot05_06(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    obj->field_225 = 1;
    obj->vel_x = data_801dd394_slot05_06[obj->direction_index];
    obj->accel_x = data_801dd398_slot05_06[obj->direction_index];
    /* The second argument is narrowed at the call, as the callers of func_801307e0 that mask it do: without the cast this function differs from the original in 7 instruction lines. */
    func_801307e0(obj, (u16)((obj->direction_index >> 1) + 0x1b));
}
