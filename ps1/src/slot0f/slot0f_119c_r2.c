/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

long write(long fd, void *buf, long n);
void func_800e146c_slot0f(Slot0fRece664 *src, Slot0fRece664 *dst);
void func_800e1580_slot0f(Slot0fRece5c4 *src, Slot0fRece5c4 *dst);
void func_800e1618_slot0f(u8 *src, u8 *dst);

extern u8 *data_800e8504_slot0f;

void func_800e12cc_slot0f(long fd, void *buf, long n) {
    int i = 0x78;
    do {
        func_8015fb30(0);
        if (write(fd, buf, n) != -1) {
            break;
        }
    } while (--i != 0);
}

void func_800e1348_slot0f(void) {
    func_800e146c_slot0f((Slot0fRece664 *)(data_800e8504_slot0f + 0x100), (Slot0fRece664 *)table_8016e664);
    func_800e1580_slot0f((Slot0fRece5c4 *)(data_800e8504_slot0f + 0x130), (Slot0fRece5c4 *)table_8016e5c4);
    func_800e1580_slot0f((Slot0fRece5c4 *)(data_800e8504_slot0f + 0x190), (Slot0fRece5c4 *)table_8016e614);
    func_800e1618_slot0f(data_800e8504_slot0f + 0x1f0, (u8 *)&game_state + 0x15e);
    *(u16 *)((u8 *)&game_state + 0x21e) = *(u16 *)(data_800e8504_slot0f + 0x230);
}
