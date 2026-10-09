/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800e8480_slot0f[];
extern Slot0fRec8490 data_800e8490_slot0f[];
extern s8 data_800f8576_slot0f;

/* The 16-bit local w holds the constant 3 and then 0x1b. Written with the two literals, this function differs from the original in 11 instruction slots; with w as an int, in 17. */
void func_800e07b8_slot0f(int sel) {
    int i;
    s16 w;
    Slot0fRec8490 *p;
    func_801519b4((Object *)data_800e8480_slot0f);
    for (i = 0; i < 6; i++) {
        p = &data_800e8490_slot0f[i];
        p->field_0b = sel == i ? 0x10 : 0x1a;
        if (i == 1) {
            w = 3;
            if (data_800f8576_slot0f != w) {
                w = 0x1b;
                p->field_0b = w;
            }
        }
        func_801519b4((Object *)p);
    }
}
