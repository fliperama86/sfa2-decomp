/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Not in protos.h. */
extern void func_8015c09c(void *prim);
extern int func_8015bdd4(int a, int b);

void func_801e0d94_slot2a(Object *obj, SlotCell *cell, s16 value) {
    SlotList *list = (SlotList *)obj->sequence->field_04;
    int n = list->count;
    int c = 0x80;
    u8 *p = list->entries;

    do {
        func_8015c09c(cell);
        p++;
        cell->field_04 = c;
        cell->field_05 = c;
        cell->field_06 = c;
        cell->field_16 = value;
        cell->field_0e = (s16)func_8015bdd4((*p++ << 4) + 0x2c0, 0x60);
        p += 2;
        cell++;
        n--;
    } while (n > 0);
}
