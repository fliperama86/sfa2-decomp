/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_800f8558_slot0f;
extern Slot0fRec9540 data_800e9600_slot0f[];

void func_800e2cdc_slot0f(void) {
    s8 *p;
    s8 *q;
    int i;
    if ((data_801a696a | data_801a6976) & 0x2000) {
        p = &data_800f8558_slot0f;
        *p = (*p + 1 < 8) ? *p + 1 : 0;
    } else if ((data_801a696a | data_801a6976) & 0x8000) {
        p = &data_800f8558_slot0f;
        *p = (*p - 1 >= 0) ? *p - 1 : 7;
    } else {
        return;
    }
    i = 0;
    q = &data_800f8558_slot0f;
    for (; i < 8; i++) {
        if (*q >= i) {
            data_800e9600_slot0f[i].field_07 = 0x16;
        } else {
            data_800e9600_slot0f[i].field_07 = 0x1b;
        }
    }
    func_80120554(0, 0, 0x203);
}

extern u8 data_800f855a_slot0f;
extern Slot0fRec94ec data_800e951c_slot0f[];
extern Slot0fRec94ec *data_800e96a8_slot0f;

void func_800e2dcc_slot0f(void) {
    u8 *p;
    if ((data_801a696a | data_801a6976) & 0xa000) {
        p = &data_800f855a_slot0f;
        *p ^= 1;
        data_800e96a8_slot0f = &data_800e951c_slot0f[(s8)*p];
        func_80120554(0, 0, 0x203);
    }
}
