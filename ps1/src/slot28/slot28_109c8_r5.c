/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800445f0_slot28[];
extern u8 data_80051b58_slot28;
extern u8 data_80051b5c_slot28;
extern u8 data_80051b60_slot28;
extern ObjectRef data_80051b48_slot28;

void func_80020fa4_slot28(Object *obj) {
    int m;
    int i = data_80051b58_slot28;
    if (i != 0) {
        data_80051b5c_slot28--;
        if (data_80051b5c_slot28 == 0) {
            data_80051b5c_slot28 = 0x20;
            data_80051b58_slot28 = 1;
        }
        i = data_80051b58_slot28;
    }
    m = obj->field_32;
    if ((data_800445f0_slot28[i] & m) == 0) {
        data_80051b60_slot28 ^= 1;
    }
    {
        int x = data_80051b60_slot28;
        data_80051b48_slot28.p->pos_x = x ? -0x148 : 0x58;
    }
}
