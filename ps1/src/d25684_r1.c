/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

/* The original loads one of two halfwords, then tests a register that it has just set to zero, and keeps the code of the arm that this test never reaches. The test of the high half of the 16-bit local gives that; what the original's source tested there is not known. Written as w == 0 instead, this function differs from the original in 7 instruction slots. The local o2 holds the pointer read at the top: with the read at its use, 8 instruction slots differ. */
void func_80125684(void) {
    Object *o = game_state.field_78;
    Object *o2 = game_state.field_7c;
    u16 w;
    if (o->side != 0) {
        w = data_801a6972;
    } else {
        w = data_801a6966;
    }
    if ((w >> 16) == 0) {
        game_state.field_139--;
        if (game_state.field_139 & 0x80) {
            game_state.field_138++;
            o->field_2ac = o2->kind;
        }
    } else {
        game_state.field_138 = 0;
        game_state.field_139 = 0x3c;
        game_state.field_13a = 0;
    }
}

