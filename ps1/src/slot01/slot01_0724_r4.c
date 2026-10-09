/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern char data_800100a0_slot01[];
char *strcat(char *dest, const char *src);

void func_80010bf0_slot01(int value, char *p) {
    int started = 0;
    int i = 0;
    int div = 100000;
    char *s;

    s = p;

    if (value == 0) {
        s[0] = 0x20;
        s[1] = 0x20;
        s[2] = 0x20;
        s[3] = 0x20;
        s[4] = 0x20;
        s[5] = 0x30;
        p = s + 6;
    } else {
        do {
            int digit;
            if (value == 0) {
                *p++ = 0x30;
                break;
            }
            digit = value / div;
            value = value - digit * div;
            if (started == 0) {
                if (digit != 0) {
                    *p++ = digit + 0x30;
                    started = 1;
                } else {
                    *p++ = 0x20;
                }
            } else {
                *p++ = digit + 0x30;
            }
            div = div / 10;
            i++;
        } while (i < 6);
    }
    *p = 0;
    strcat(s, data_800100a0_slot01);
}
