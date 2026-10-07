/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e146c_slot0f(Slot0fRece664 *src, Slot0fRece664 *dst);
void func_800e1580_slot0f(Slot0fRece5c4 *src, Slot0fRece5c4 *dst);
void func_800e1618_slot0f(u8 *src, u8 *dst);

extern u8 *data_800e8504_slot0f;

void func_800e13dc_slot0f(void) {
    func_800e146c_slot0f((Slot0fRece664 *)table_8016e664, (Slot0fRece664 *)(data_800e8504_slot0f + 0x100));
    func_800e1580_slot0f((Slot0fRece5c4 *)table_8016e5c4, (Slot0fRece5c4 *)(data_800e8504_slot0f + 0x130));
    func_800e1580_slot0f((Slot0fRece5c4 *)table_8016e614, (Slot0fRece5c4 *)(data_800e8504_slot0f + 0x190));
    func_800e1618_slot0f((u8 *)&game_state + 0x15e, data_800e8504_slot0f + 0x1f0);
    *(u16 *)(data_800e8504_slot0f + 0x230) = *(u16 *)((u8 *)&game_state + 0x21e);
}
