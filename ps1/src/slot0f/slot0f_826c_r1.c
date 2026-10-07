/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800f01e8_slot0f[];

void func_800e826c_slot0f(u8 *buf, unsigned int n, int pos, int flags) {
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

void func_800e8314_slot0f(u8 *buf, unsigned int n, int pos, int flags) {
    buf += pos;
    *buf = 0;
    pos--;
    buf--;
    while (pos >= 0) {
        u8 c;
        if (n == 0) break;
        c = data_800f01e8_slot0f[n & 0xf];
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
