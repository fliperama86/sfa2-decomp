/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_80028a40_slot12[];
extern Slot12Prim data_8002c528_slot12[];
extern HudSlot data_8002c4c8_slot12[4];
extern Slot12Prim data_8002c3c8_slot12[];

void func_80016ff4_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int i;
    int idx;

    if (obj->field_5c >= 0) {
        i = 0;
        idx = data_801a27d0;
        do {
            ((PrimTag *)&((Slot12Prim (*)[2])data_8002c528_slot12)[i][idx])->addr = ((PrimTag *)data_801987c8)[data_80028a40_slot12[obj->field_09]].addr;
            ((PrimTag *)data_801987c8)[data_80028a40_slot12[obj->field_09]].addr = (u32)&((Slot12Prim (*)[2])data_8002c528_slot12)[i][idx];
            i++;
        } while (i <= obj->field_5c);
    }
    for (i = 0; i < 4; i++) {
        func_801519b4(&data_8002c4c8_slot12[i]);
        ((PrimTag *)((u8 *)data_8002c3c8_slot12 + (data_801a27d0 << 5) + (i << 6)))->addr = ((PrimTag *)data_801987c8)[data_80028a40_slot12[obj->field_09]].addr;
        ((PrimTag *)data_801987c8)[data_80028a40_slot12[obj->field_09]].addr = (u32)((u8 *)data_8002c3c8_slot12 + (i << 6) + (data_801a27d0 << 5));
    }
}
