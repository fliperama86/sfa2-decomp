/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_800f0204_slot0f;
extern int data_800f8578_slot0f;
extern s8 data_800f855e_slot0f;
extern Rect data_800e934c_slot0f[];
extern u8 data_800e96dc_slot0f[];
extern Rect *data_800e96e8_slot0f;

void func_800e31d4_slot0f(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    u8 *p;
    s8 *q;
    s8 sel;
    int cnt;
    u16 pad = data_801a696a | data_801a6976;

    if (pad & 0x20) {
        data_800f8578_slot0f = 1;
        func_80120554((Object *)0, 0, 0x205);
    }
    cnt = data_800f0204_slot0f + 1;
    data_800f0204_slot0f = cnt;
    if (cnt == 8) {
        data_800f0204_slot0f = 0;
        q = &data_800f855e_slot0f;
        *q = (*q + 1 < 7) ? *q + 1 : 0;
        sel = data_800f855e_slot0f;
        data_800e96e8_slot0f = &data_800e934c_slot0f[sel];
    }
    p = data_800e96dc_slot0f;
    func_801519b4(p);
    if ((u8)(data_800f855e_slot0f - 2) < 3) {
        func_801519b4(p + 0x10);
    } else {
        func_801519b4(p + 0x20);
        func_801519b4(p + 0x30);
    }
}
