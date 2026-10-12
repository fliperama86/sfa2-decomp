/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c5758_slot04_06;

void func_801b5d24_slot04_06(Object *o) {
    Object *p = data_801c5758_slot04_06;

    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    func_8011f38c(o);
}
