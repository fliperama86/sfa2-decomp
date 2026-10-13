/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern s8 data_800f8556_slot0f;
extern Slot0fRec94ec data_800e94ec_slot0f[];
extern Slot0fRec94ec *data_800e9698_slot0f;

void update_three_option_selection_slot0f(void) {
    s8 *p;
    if ((pad_1_pressed | pad_2_pressed) & 0x2000) {
        p = &data_800f8556_slot0f;
        *p = (*p + 1 < 3) ? *p + 1 : 0;
    } else if ((pad_1_pressed | pad_2_pressed) & 0x8000) {
        p = &data_800f8556_slot0f;
        *p = (*p - 1 >= 0) ? *p - 1 : 2;
    } else {
        return;
    }
    data_800e9698_slot0f = &data_800e94ec_slot0f[data_800f8556_slot0f];
    play_sound_at(0, 0, 0x203);
}
