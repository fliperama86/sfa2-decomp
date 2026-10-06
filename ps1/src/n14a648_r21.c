/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
extern u8 data_8018d260;
extern u8 data_80181044[];
extern u8 data_80181060[];
extern u8 data_801811aa[];
extern u8 data_80181206[];
extern u8 data_8018124a[];

/* Residual (not exact): the original keeps &game_state in $s0 for the single
   field_31 read after func_801519b4 and rematerialises the constant 1; here the
   pointer is folded and the constant 1 lives in $s0 (20 slots, 4 bytes shorter).
   Tried: Config/game_state_second pointer, u8 pointer, ternary vs if for the
   argument, local for field_31. Everything else matches. */
void func_80154978(void) {
    data_8018f5a0->field_52++;
}
