/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_800f8557_slot0f;
extern Slot0fRec9540 data_800e95c0_slot0f[];

void func_800e2bec_slot0f(void) {
    s8 *p;
    s8 *q;
    int i;
    if ((data_801a696a | data_801a6976) & 0x2000) {
        p = &data_800f8557_slot0f;
        *p = (*p + 1 < 4) ? *p + 1 : 0;
    } else if ((data_801a696a | data_801a6976) & 0x8000) {
        p = &data_800f8557_slot0f;
        *p = (*p - 1 >= 0) ? *p - 1 : 3;
    } else {
        return;
    }
    i = 0;
    q = &data_800f8557_slot0f;
    for (; i < 4; i++) {
        if (*q >= i) {
            data_800e95c0_slot0f[i].field_07 = 0x16;
        } else {
            data_800e95c0_slot0f[i].field_07 = 0x1b;
        }
    }
    func_80120554(0, 0, 0x203);
}
