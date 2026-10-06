/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot2aBank data_801e2fdc_slot2a[];
extern u32 data_801e515c_slot2a;
extern void (*data_801e2300_slot2a[])(Object *);
extern void (*data_801e230c_slot2a[])(Object *);
void func_801e12fc_slot2a(Object *obj);
void func_801e16a8_slot2a(Object *obj);

/* The masked copy of p is taken first in the loop. With it the compiler loads the mask 0xffffff before 0xff000000, as the original does. */
void func_801e114c_slot2a(Object *obj) {
    u32 i;
    u32 *p = (u32 *)((u8 *)data_801e2fdc_slot2a + obj->field_03 * 0x10c0 + data_801a27d0 * 0x860);
    u32 *q = (u32 *)((u8 *)data_801987c8 + 8);
    u32 n = data_801e515c_slot2a;
    u32 pa;
    for (i = 0; i < n; i++) {
        pa = (u32)p & 0xffffff;
        *p = (*p & 0xff000000) | (*q & 0xffffff);
        *q = (*q & 0xff000000) | pa;
        p += 5;
    }
    *p = (*p & 0xff000000) | (*q & 0xffffff);
    *q = (*q & 0xff000000) | ((u32)p & 0xffffff);
}
