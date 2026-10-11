/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c5398_slot04_06[];
extern s32 data_801c539c_slot04_06[];

/* What it does (inferred): enters a character state. It clears the bytes
   field_12c to field_12f, sets field_17b to 1, adds 1 to state_3, calls
   func_80141f28 (object, 7) and func_80138ae8 (&game_state, object), sets
   field_225 to 1, loads vel_x and accel_x from two word tables indexed
   by the byte field_12a (they lie one word apart), and starts sequence
   (field_12a >> 1) + 0x1b. */
void func_801b20b0_slot04_06(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_17b = 1;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    obj->field_225 = 1;
    obj->vel_x = data_801c5398_slot04_06[obj->field_12a];
    obj->accel_x = data_801c539c_slot04_06[obj->field_12a];
    /* The second argument is narrowed at the call, as the callers of func_801307e0 that mask it do: without the cast this function differs from the original in 7 instruction lines. */
    func_801307e0(obj, (u16)((obj->field_12a >> 1) + 0x1b));
}
