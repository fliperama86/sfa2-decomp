/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800125f4_slot01(u8 *src, u8 unused) {
    Rect r;
    u8 *p;
    r.x = 0x70;
    r.y = 0x1ec;
    r.w = 0x10;
    r.h = 3;
    func_80157fc4(&r, src);
    p = data_801a2964;
    func_80158028(&r, p);
    p += 0x1400;
    func_80158028(&r, p);
    func_80157d9c(0);
}
