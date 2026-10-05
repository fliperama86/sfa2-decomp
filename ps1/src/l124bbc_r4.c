/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80125dc0(u8 bank, u8 count, u8 scale, u8 row) {
    u16 n;
    u16 k;
    s16 r;
    s16 g;
    s16 b;
    u16 p;
    u16 flag;
    for (n = 0; n < count; n++) {
        for (k = 1; k < 16; k++) {
            p = data_801a27e4_rows[bank][row * 16 + k];
            flag = p & 0x8000;
            p = data_801a27e4_rows[bank + 5][row * 16 + k];
            r = (p & 0x1f) * scale / 31;
            g = ((p >> 5) & 0x1f) * scale / 31;
            b = ((p >> 10) & 0x1f) * scale / 31;
            if (r == 0 && g == 0 && b == 0) {
                r++;
                g++;
                b++;
            }
            data_801a27e4_rows[bank][row * 16 + k] = flag + (r + ((b << 10) + (g << 5)));
        }
        row++;
    }
}

void func_80125f5c(u8 bank, u8 count, u8 scale, u8 row) {
    u16 n;
    u16 k;
    int r;
    int g;
    int b;
    u16 p;
    u16 flag;
    for (n = 0; n < count; n++) {
        for (k = 0; k < 16; k++) {
            p = data_801a27e4_rows[bank][row * 16 + k];
            flag = p & 0x8000;
            p = data_801a27e4_rows[bank + 5][row * 16 + k];
            r = (p & 0x1f) * scale / 31;
            g = ((p >> 5) & 0x1f) * scale / 31;
            b = ((p >> 10) & 0x1f) * scale / 31;
            data_801a27e4_rows[bank][row * 16 + k] = flag + (r + ((b << 10) + (g << 5)));
        }
        row++;
    }
}
