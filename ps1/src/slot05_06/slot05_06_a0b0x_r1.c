/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801dd394_slot05_06[];
extern s32 data_801dd398_slot05_06[];

/* What it does (inferred): enters a character state. It clears the bytes
   field_12c to field_12f, sets field_17b to 1, adds 1 to field_07, calls
   func_80141f28 (object, 7) and func_80138ae8 (&game_state, object), sets
   field_225 to 1, loads field_4c and field_54 from two word tables indexed
   by the byte field_12a (they lie one word apart), and starts sequence
   (field_12a >> 1) + 0x1b. */
void func_801ca0b0_slot05_06(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    obj->field_225 = 1;
    obj->field_4c = data_801dd394_slot05_06[obj->field_12a];
    obj->field_54 = data_801dd398_slot05_06[obj->field_12a];
    /* The second argument is narrowed at the call, as the callers of func_801307e0 that mask it do: without the cast this function differs from the original in 7 instruction lines. */
    func_801307e0(obj, (u16)((obj->field_12a >> 1) + 0x1b));
}
