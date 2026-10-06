/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80022e48_slot12[];
extern Slot12Tile data_8002a394_slot12[];
extern Slot12Tile data_8002a794_slot12[];

void func_80012b5c_slot12(Object *obj) {
    u16 *t = data_80022e48_slot12;
    u32 *ot = (u32 *)((u8 *)data_801987c8 + 0x3c);
    Slot12Tile *p;
    int i;

    if (obj->field_03 == 0) {
        p = data_8002a394_slot12 + data_801a27d0 * 0x20;
    } else {
        p = data_8002a794_slot12 + data_801a27d0 * 0x20;
    }
    for (i = 0; i < 14; i++) {
        p->x = obj->pos_x + *t++;
        p->y = obj->pos_y + *t++;
        ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)p;
        p++;
    }
}
