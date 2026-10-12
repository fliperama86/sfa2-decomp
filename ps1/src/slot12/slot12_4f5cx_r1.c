/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_800282dc_slot12[];
extern Slot12Cell data_8002bc38_slot12[];

void func_80014f5c_slot12(Object *obj) {
    u32 *ot = (u32 *)data_801987c8;
    u32 *cell = (u32 *)(data_8002bc38_slot12 + obj->table_index * 2 + data_801a27d0);
    ((PrimTag *)cell)->addr = ((PrimTag *)&ot[data_800282dc_slot12[obj->ot_depth]])->addr;
    ((PrimTag *)&ot[data_800282dc_slot12[obj->ot_depth]])->addr = (u32)cell;
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
}
