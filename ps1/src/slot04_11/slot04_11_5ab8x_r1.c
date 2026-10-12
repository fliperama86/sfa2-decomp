/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04_11Rec785c data_801c785c_slot04_11[];

void copy_kind_table_to_scratch_by_side_slot04_11(Object *obj);

void copy_kind_table_to_scratch_by_side_slot04_11(Object *obj) {
    u32 *dst;
    Slot04_11Rec785c *p;
    int k;
    int i;

    k = 2;
    if (obj->kind == 0x11) {
        k = 1;
    }
    dst = (u32 *)0x1f800100;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    for (i = 0; i < 15; i++) {
        p = &data_801c785c_slot04_11[i];
        dst[data_801c785c_slot04_11[i].a] = (&p->a)[k];
    }
}
