/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_800e92a7_slot0f[];
extern int data_800f0204_slot0f;

void update_eleven_entry_menu_cursor_slot0f(void) {
    HudState *h = screen_state;
    u16 pad = pad_1_pressed | pad_2_pressed;
    int sel = h->substep_index;
    int i;
    u8 c;
    if (pad & 0x1000) {
        int v = sel - 1;
        HudState *q;
        u16 cur;
        q = h;
        if (v < 0) {
            h->substep_index = 10;
        } else {
            h->substep_index = v;
        }
        cur = q->substep_index;
        q->substep_index = cur;
        sel = cur;
    } else if (pad & 0x4000) {
        int v = sel + 1;
        HudState *q;
        u16 cur;
        q = h;
        if (v < 11) {
            h->substep_index = v;
        } else {
            h->substep_index = 0;
        }
        cur = q->substep_index;
        q->substep_index = cur;
        sel = cur;
    } else if (pad & 4) {
        if (sel == 10) {
            sel = 5;
            h->substep_index = 5;
        } else {
            sel -= 6;
            if (sel >= 0) {
                h->substep_index = sel;
            } else {
                sel += 6;
            }
        }
    } else if (pad & 8) {
        if (sel == 5) {
            sel = 10;
            h->substep_index = 10;
        } else {
            sel += 6;
            if (sel < 11) {
                h->substep_index = sel;
            } else {
                sel -= 6;
            }
        }
    } else {
        return;
    }
    data_800f0204_slot0f = 0;
    c = 0x1a;
    for (i = 0xa0; i >= 0; i -= 0x10) {
        data_800e92a7_slot0f[i] = c;
    }
    data_800e92a7_slot0f[sel * 16] = 0x10;
    play_sound_at((Object *)0, 0, 0x204);
}
