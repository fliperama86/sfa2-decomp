/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80028508_slot12[];

void format_decimal_padded_slot12(u8 *buf, unsigned int n, int pos, int flags) {
    buf += pos;
    *buf = 0;
    pos--;
    buf--;
    while (pos >= 0) {
        unsigned int q;
        if (n == 0) break;
        q = n / 10;
        pos--;
        *buf = n - q * 10 + 0x30;
        buf--;
        n = q;
    }
    if (flags & 1) {
        while (pos >= 0) {
            *buf = 0x30;
            pos--;
            buf--;
        }
    } else {
        while (pos >= 0) {
            *buf = 0x20;
            pos--;
            buf--;
        }
    }
}

void format_hex_padded_slot12(u8 *buf, unsigned int n, int pos, int flags) {
    buf += pos;
    *buf = 0;
    pos--;
    buf--;
    while (pos >= 0) {
        u8 c;
        if (n == 0) break;
        c = data_80028508_slot12[n & 0xf];
        n >>= 4;
        pos--;
        *buf = c;
        buf--;
    }
    if (flags & 1) {
        while (pos >= 0) {
            *buf = 0x30;
            pos--;
            buf--;
        }
    } else {
        while (pos >= 0) {
            *buf = 0x20;
            pos--;
            buf--;
        }
    }
}
