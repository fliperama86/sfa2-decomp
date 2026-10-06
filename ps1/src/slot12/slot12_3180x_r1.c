/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 *data_8002b614_slot12;
extern u32 *data_8002b618_slot12;
extern u8 data_8002ab94_slot12[];

void func_80013180_slot12(void) {
    data_8002b614_slot12 = (u32 *)(data_8002ab94_slot12 + data_801a27d0 * 0x540);
    data_8002b618_slot12 = (u32 *)((u8 *)data_801987c8 + 0x24);
    func_80158a2c((Prim *)data_8002b614_slot12, 0, 0, 0x16, 0);
    {
        u32 *p = data_8002b614_slot12;
        data_8002b614_slot12 = p + 3;
        ((PrimTag *)p)->addr = (u32)data_8002b614_slot12;
    }
}
