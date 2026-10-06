/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SlotCell data_80029350_slot12[];
extern SlotCell data_80029b70_slot12[];
int func_8015bdd4(int a, int b);
void func_8015c09c(SlotCell *cell);

void func_800123e0_slot12(void) {
    int i;

    for (i = 0; i < 0x34; i++) {
        SlotCell *p = &data_80029350_slot12[i];
        SlotCell *q = &data_80029350_slot12[i + 0x34];

        func_8015c09c(p);
        func_8015c09c(q);
        p->field_04 = 0x80;
        p->field_05 = 0x80;
        p->field_06 = 0x80;
        q->field_04 = 0x80;
        q->field_05 = 0x80;
        q->field_06 = 0x80;
        data_80029350_slot12[i].field_16 = func_8015bd0c(0, 0, 0x3c0, 0);
        data_80029b70_slot12[i].field_16 = func_8015bd0c(0, 0, 0x3c0, 0);
        data_80029350_slot12[i].field_0e = func_8015bdd4(0, 0x1f0);
        data_80029b70_slot12[i].field_0e = func_8015bdd4(0, 0x1f0);
    }
}
