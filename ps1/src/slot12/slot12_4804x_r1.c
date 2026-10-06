/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8002b710_slot12[16];
extern u16 data_8002bc30_slot12;
extern void (*table_800281f0_slot12[])(Object *);
int func_800144f8_slot12(u8 a, int b, u16 *dst, u16 *src);
void func_80013cf8_slot12(void);
void func_80013c74_slot12(void);

u8 func_80014804_slot12(void) {
    u16 *dst;
    u16 *src;
    int i;
    u16 *p;
    u16 *base;
    int r;
    i = 0;
    base = (u16 *)data_8002b710_slot12 + 21 * 16;
    src = base - 0x140;
    dst = base;
    for (; i < 0x14; i++) {
        func_800144f8_slot12(0, -1, dst, src);
        src += 16;
        dst += 16;
    }
    func_80013cf8_slot12();
    p = (u16 *)data_8002b710_slot12;
    data_8002bc30_slot12 += 0x14;
    r = func_800144f8_slot12(1, -1, p, p - 16) & 0xff;
    func_80013c74_slot12();
    return r;
}
