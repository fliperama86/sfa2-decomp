/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

extern u16 pad_1_held;

/* The two parameters are not used: the function reads the words of both pads itself. Its one
   caller, the game's main, passes a pad's buffer and that pad's number at each of its two calls
   (what the original declaration was is not known). */
int count_pad_records_with_all_buttons_and_flag(u8 *pad, int number) {
    int n = (pad_1_held & 0x10f) == 0x10f && (pad_1_pressed & 0x800) != 0;
    if ((pad_2_held & 0x10f) == 0x10f) {
        if (pad_2_pressed & 0x800) {
            n++;
        }
    }
    return n;
}
