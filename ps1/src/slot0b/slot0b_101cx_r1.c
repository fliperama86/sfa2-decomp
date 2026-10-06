/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8015c09c(void *prim);

SlotCell *func_801e101c_slot0b(Object *obj, SlotCell *cell, int arg, int base) {
    Slot0bList *list = (Slot0bList *)obj->sequence->field_04;
    u8 *p;
    s16 a2 = arg;
    int b2 = base;
    int n;
    int i;
    for (i = 0; i < 2; i++) {
        n = list->count;
        p = list->data;
        do {
            func_8015c09c(cell);
            p++;
            cell->field_04 = 0x80;
            cell->field_05 = 0x80;
            cell->field_06 = 0x80;
            cell->field_16 = a2;
            cell->field_0e = b2 + (*p++ << 6);
            p += 2;
            cell++;
            n--;
        } while (n > 0);
    }
    return cell;
}
