/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801a2fe4[];
extern u16 data_8004897c_slot28[];

void func_80022bf0_slot28(void) {
    int i = 0;
    u16 *base = (u16 *)data_801a2fe4;
    u16 *b = base + 0xa00;
    u16 *s = data_8004897c_slot28;
    u16 *a = base;
    for (; i < 0x1a0; i++) {
        *a = *s;
        *b++ = *s;
        s++;
        a++;
    }
}
