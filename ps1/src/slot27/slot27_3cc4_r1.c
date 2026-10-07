/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot27Rec9328 data_80029328_slot27[];
extern u16 data_801a2b84[];

void func_80013cc4_slot27(Object *object) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int i;
    u16 w;
    int n;
    Slot27Rec9328 *tab = data_80029328_slot27;
    s16 c;

    c = data_80029328_slot27[ref_other.p->side].count;
    c -= 1;
    data_80029328_slot27[ref_other.p->side].count = c;
    if (c == 0) {
        tab[ref_other.p->side].cur = data_80029328_slot27[ref_other.p->side].cur + 2;
        if (*(s32 *)data_80029328_slot27[ref_other.p->side].cur < 0) {
            data_80029328_slot27[ref_other.p->side].cur = (u16 *)*(s32 *)data_80029328_slot27[ref_other.p->side].cur;
        }
        n = *(s16 *)(data_80029328_slot27[ref_other.p->side].cur + 1);
        data_80029328_slot27[ref_other.p->side].count = n;
        data_80029328_slot27[ref_other.p->side].field_06 = data_80029328_slot27[ref_other.p->side].cur[0];
    }
    w = data_80029328_slot27[ref_other.p->side].field_06;
    object->field_22 = w & 0x8000;
    for (i = 0; i < 16; i++) {
        data_801a2b84[ref_other.p->side * 16 + i] = ((u16 (*)[16])0x800ed800)[w & 0x1f][i];
        data_801a2b84[ref_other.p->side * 16 + i + 0xa00] = ((u16 (*)[16])0x800ed800)[w & 0x1f][i];
    }
    func_80137220(0, 0);
}
