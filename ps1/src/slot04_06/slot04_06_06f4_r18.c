/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c5758_slot04_06;
void func_8011f38c(Object *o);

void func_801b5d24_slot04_06(Object *o) {
    Object *p = data_801c5758_slot04_06;

    if ((s32)o == p->field_28) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}
