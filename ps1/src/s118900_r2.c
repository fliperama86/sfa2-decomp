/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

/* The two parameters are not used: the function reads the words of both pads itself. Its one
   caller, the game's main, passes a pad's buffer and that pad's number at each of its two calls
   (what the original declaration was is not known). */
int count_pad_records_with_all_buttons_and_flag(u8 *pad, int number) {
    int n = (data_801a6966 & 0x10f) == 0x10f && (data_801a696a & 0x800) != 0;
    if ((data_801a6972 & 0x10f) == 0x10f) {
        if (data_801a6976 & 0x800) {
            n++;
        }
    }
    return n;
}
