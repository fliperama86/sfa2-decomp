/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800f855b_slot0f;
extern u8 data_800f8559_slot0f;
extern Slot0fRec94ec data_800e951c_slot0f[];
extern Slot0fRec94ec data_800e950c_slot0f[];
extern Slot0fRec94ec *data_800e96b8_slot0f;
extern Slot0fRec94ec *data_800e96c8_slot0f;

void func_800e2e44_slot0f(void) {
    u8 *p;
    if ((data_801a696a | data_801a6976) & 0xa000) {
        p = &data_800f855b_slot0f;
        *p ^= 1;
        data_800e96b8_slot0f = &data_800e951c_slot0f[(s8)*p];
        func_80120554(0, 0, 0x203);
    }
}

void func_800e2ebc_slot0f(void) {
    u8 *p;
    if ((data_801a696a | data_801a6976) & 0xa000) {
        p = &data_800f8559_slot0f;
        *p ^= 1;
        data_800e96c8_slot0f = &data_800e950c_slot0f[(s8)*p];
        func_80120554(0, 0, 0x203);
    }
}
