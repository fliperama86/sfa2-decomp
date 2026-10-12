/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* Inferred, not an original name: clears 128 bytes of game_state starting at
   kind_index, then clears the byte at field_226. */
void clear_game_state_fields(void) {
    u8 *p = (u8 *)&game_state.kind_index;
    unsigned int i;

    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    for (i = 0; i < 0x80; i++)
        do {
            *p++ = 0;
        } while (0);
    game_state.field_226 = 0;
}
