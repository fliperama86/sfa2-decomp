/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014434_slot12(Object *obj, Slot12Prim *c, int x, int y) {
    int i;
    u8 *p = (u8 *)c + 0xc;
    for (i = 0; i < 2; i++, c++) {
        u8 *q;
        func_80158a2c((Prim *)c, 1, 0, 0, 0);
        q = p + i * 0x20;
        q[3] = 4;
        q[7] = 0x64;
        q[4] = 0x80;
        q[5] = 0x80;
        q[6] = 0x80;
        *(u16 *)(q + 0x10) = x;
        *(u16 *)(q + 0x12) = y;
        func_8015c23c((Tx *)c, q);
    }
}
