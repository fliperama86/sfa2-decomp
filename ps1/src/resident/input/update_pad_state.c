/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

extern u16 pad_1_held;

void update_pad_state(void) {
    int cur;
    int old;
    int oldnew;
    cur = 0;
    if (data_801a89a8[0] == 0 && data_801a89a8[1] == 0x41) {
        cur = ~(data_801a89a8[3] | (data_801a89a8[2] << 8));
    }
    old = pad_1_held;
    oldnew = pad_1_pressed;
    pad_1_held = cur;
    data_801a6968 = old;
    data_801a696c = oldnew;
    pad_1_pressed = cur & ~old;
    if (data_801a89a8[4] == 0 && data_801a89a8[5] == 0x41) {
        cur = ~(data_801a89a8[7] | (data_801a89a8[6] << 8));
    } else {
        cur = 0;
    }
    old = pad_2_held;
    oldnew = pad_2_pressed;
    pad_2_held = cur;
    data_801a6974 = old;
    pad_2_pressed = cur & ~old;
    data_801a6978 = oldnew;
    if (pad_2_pressed & 0x800) {
        data_801ac61c = ~data_801ac61c & 1;
    }
}
