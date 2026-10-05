/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s16 data_801aa4dc[];

int func_80127cd8(u8 bank, s16 amount, u8 mode) {
    u16 n;
    u16 k;
    s16 r;
    s16 g;
    s16 b;
    s16 r2;
    s16 g2;
    s16 b2;
    u16 p;
    u16 flag;
    for (n = 0; n < 32; n++) {
        for (k = 1; k < 16; k++) {
            p = data_801a27e4_rows[bank][n * 16 + k];
            r = p & 0x1f;
            g = (p >> 5) & 0x1f;
            b = (p >> 10) & 0x1f;
            flag = p & 0x8000;
            switch (mode) {
            case 0:
                r = r - 1;
                g = g - 1;
                b = b - 1;
                if (r <= 0) r = 1;
                if (g <= 0) g = 1;
                if (b <= 0) b = 1;
                break;
            case 1:
                p = data_801a27e4_rows[bank + 5][n * 16 + k];
                r2 = p & 0x1f;
                g2 = (p >> 5) & 0x1f;
                b2 = (p >> 10) & 0x1f;
                if (data_801aa4dc[bank] <= 0) data_801aa4dc[bank] = 0;
                r = r2 * data_801aa4dc[bank] / 31;
                g = g2 * data_801aa4dc[bank] / 31;
                b = b2 * data_801aa4dc[bank] / 31;
                if (r2 < r) r = r2;
                if (g2 < g) g = g2;
                if (b2 < b) b = b2;
                if (b == 0 && g == 0 && r == 0) {
                    r++;
                    g++;
                    b++;
                }
                break;
            case 2:
                r = r + 1;
                g = g + 1;
                b = b + 1;
                if (r >= 31) r = 31;
                if (g >= 31) g = 31;
                if (b >= 31) b = 31;
                break;
            case 3:
                p = data_801a27e4_rows[bank + 5][n * 16 + k];
                r = r - 1;
                g = g - 1;
                b = b - 1;
                r2 = p & 0x1f;
                g2 = (p >> 5) & 0x1f;
                b2 = (p >> 10) & 0x1f;
                if (r < r2) r = r2;
                if (g < g2) g = g2;
                if (b < b2) b = b2;
                if (b == 0 && g == 0 && r == 0) {
                    r++;
                    g++;
                    b++;
                }
                break;
            }
            data_801a27e4_rows[bank][n * 16 + k] = flag + (r + ((b << 10) + (g << 5)));
        }
    }
    data_801aa4dc[bank] += amount;
    if (data_801aa4dc[bank] < 0 && mode == 0) {
        return 1;
    } else if (data_801aa4dc[bank] >= 32 && mode == 1) {
        return 1;
    }
    return 0;
}
