/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e5160_slot2a[];
extern Slot2aSlot data_801e5164_slot2a[][2];
extern u16 *data_801e52c8_slot2a;

void func_801e16a8_slot2a(Object *obj) {
    u32 *link = (u32 *)data_801987c8 + 1;
    Rec14 *rec;
    u32 tail;
    int i;
    rec = (Rec14 *)((u8 *)data_801e5164_slot2a + obj->field_03 * 0xb0 + data_801a27d0 * 0x58);
    for (i = 0; i < 2; i++) {
        if (data_801e5160_slot2a[obj->field_03] == 0) {
            rec->field_0d = *(u8 *)data_801e52c8_slot2a * 0x30;
        }
        ((PrimTag *)rec)->addr = ((PrimTag *)link)->addr;
        ((PrimTag *)link)->addr = (u32)rec;
        tail = *link;
        rec++;
    }
    ((PrimTag *)rec)->addr = tail;
    ((PrimTag *)link)->addr = (u32)rec;
}
