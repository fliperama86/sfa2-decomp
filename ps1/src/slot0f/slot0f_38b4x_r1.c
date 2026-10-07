/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_800e92a7_slot0f[];
extern int data_800f0204_slot0f;

void func_800e38b4_slot0f(void) {
    HudState *h = data_8018f5a0;
    u16 pad = data_801a696a | data_801a6976;
    int sel = h->field_4e;
    int i;
    u8 c;
    if (pad & 0x1000) {
        int v = sel - 1;
        HudState *q;
        u16 cur;
        q = h;
        if (v < 0) {
            h->field_4e = 10;
        } else {
            h->field_4e = v;
        }
        cur = q->field_4e;
        q->field_4e = cur;
        sel = cur;
    } else if (pad & 0x4000) {
        int v = sel + 1;
        HudState *q;
        u16 cur;
        q = h;
        if (v < 11) {
            h->field_4e = v;
        } else {
            h->field_4e = 0;
        }
        cur = q->field_4e;
        q->field_4e = cur;
        sel = cur;
    } else if (pad & 4) {
        if (sel == 10) {
            sel = 5;
            h->field_4e = 5;
        } else {
            sel -= 6;
            if (sel >= 0) {
                h->field_4e = sel;
            } else {
                sel += 6;
            }
        }
    } else if (pad & 8) {
        if (sel == 5) {
            sel = 10;
            h->field_4e = 10;
        } else {
            sel += 6;
            if (sel < 11) {
                h->field_4e = sel;
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
    func_80120554((Object *)0, 0, 0x204);
}
