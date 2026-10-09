/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800518d0_slot28[];
void func_8011f240(Slab172 *s);

void func_80016e58_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_800518d0_slot28[i] = 0;
    }
}

void func_80016e7c_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_800518d0_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_800518d0_slot28[i] = 0;
        }
    }
}
