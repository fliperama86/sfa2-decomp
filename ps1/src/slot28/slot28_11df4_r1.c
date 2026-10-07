/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051b64_slot28[];
void func_8011f240(Slab172 *s);
extern u16 data_80046588_slot28[];
extern u16 data_80045d88_slot28[];
extern u16 data_80046188_slot28[];
extern void (*data_800469bc_slot28[])(Object *);

void func_80021df4_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051b64_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_80051b64_slot28[i] = 0;
        }
    }
}

void func_80021e60_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_80046588_slot28[i];
        data_801a27e4_rows[5][i] = data_80046588_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_80045d88_slot28[i];
        data_801a27e4_rows[7][i] = data_80045d88_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_80046188_slot28[i];
        data_801a27e4_rows[8][i] = data_80046188_slot28[i];
    }
}

void func_80021f34_slot28(Object *object) {
    data_800469bc_slot28[object->field_04](object);
}
