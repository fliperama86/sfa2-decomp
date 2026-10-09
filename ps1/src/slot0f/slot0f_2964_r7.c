/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800f855c_slot0f;
extern Slot0fRec94ec data_800e952c_slot0f[];
extern Slot0fRec94ec *data_800e96d8_slot0f;

void func_800e2f34_slot0f(void) {
    u8 *p;
    u8 v;
    if ((data_801a696a | data_801a6976) & 0xa000) {
        p = &data_800f855c_slot0f;
        *p ^= 1;
        v = *p;
        game_state.field_10 = v;
        data_800e96d8_slot0f = &data_800e952c_slot0f[(s8)*p];
        data_8016e686 = v;
        func_80120374(game_state.field_10);
        func_80120554(0, 0, 0x203);
    }
}
