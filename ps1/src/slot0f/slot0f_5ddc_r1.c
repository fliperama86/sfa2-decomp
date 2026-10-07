/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_800f77ac_slot0f;
extern u16 data_800f77b0_slot0f;

int func_800e5ddc_slot0f(u8 mode, int delta, u16 *dst, u16 *src) {
    u16 n;
    s16 lvl;
    s16 r;
    s16 g;
    s16 b;
    s16 R;
    s16 G;
    s16 B;
    int stp;
    u16 c;
    s16 r2;
    s16 g2;
    s16 b2;
    for (n = 1; n < 0x10; n++, dst++) {
        c = *dst;
        R = r = c & 0x1f;
        G = g = (c >> 5) & 0x1f;
        B = b = (c >> 10) & 0x1f;
        stp = c & 0x8000;
        switch (mode) {
        case 0:
            R = r - 1;
            G = g - 1;
            B = b - 1;
            if (R <= 0) {
                R = 1;
            }
            if (G <= 0) {
                G = 1;
            }
            if (B <= 0) {
                B = 1;
            }
            break;
        case 1:
            c = *src++;
            r = c & 0x1f;
            r2 = r;
            g = (c >> 5) & 0x1f;
            g2 = g;
            b = (c >> 10) & 0x1f;
            b2 = b;
            if ((s16)data_800f77ac_slot0f <= 0) {
                data_800f77ac_slot0f = 0;
            }
            R = r * (s16)data_800f77ac_slot0f / 31;
            G = g * (s16)data_800f77ac_slot0f / 31;
            B = b * (s16)data_800f77ac_slot0f / 31;
            if (r < R) {
                R = r2;
            }
            if (g < G) {
                G = g2;
            }
            if (b < B) {
                B = b2;
            }
            break;
        }
        *dst = stp + (R + ((B << 10) + (G << 5)));
    }
    lvl = (s16)data_800f77ac_slot0f + delta;
    data_800f77ac_slot0f = lvl;
    switch (mode) {
    case 0:
        if (lvl < 0) {
            return 1;
        }
        break;
    default:
        if ((s16)data_800f77b0_slot0f < lvl) {
            return 1;
        }
        break;
    }
    return 0;
}
