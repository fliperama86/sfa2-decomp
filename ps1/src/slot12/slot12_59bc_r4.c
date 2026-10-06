/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015f00_slot12(u8 *src, u8 *dst, int mode) {
    while (*src != 0) {
        *dst++ = *src++;
    }
    switch (mode) {
    case 0:
        break;
    case 1:
        *dst++ = 'P';
        *dst++ = 'T';
        *dst++ = 'S';
        break;
    case 2:
        *dst++ = 'W';
        *dst++ = 'I';
        *dst++ = 'N';
        break;
    case 3:
        *dst++ = 'W';
        *dst++ = 'I';
        *dst++ = 'N';
        *dst++ = 'S';
        break;
    }
    *dst++ = '@';
    *dst++ = '@';
    *dst = 0;
}
