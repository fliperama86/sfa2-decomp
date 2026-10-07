/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80015420_slot01[];
extern u8 data_801a2944[];
void func_8001257c_slot01(void);

void func_8001257c_slot01(void) {
    Rect r;
    u8 *p;
    r.x = 0x70;
    r.y = 0x1eb;
    r.w = 0x10;
    r.h = 1;
    func_80157fc4(&r, data_80015420_slot01);
    p = data_801a2944;
    func_80158028(&r, p);
    func_80158028(&r, p = p + 0x1400);
    func_80157d9c(0);
}
