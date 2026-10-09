/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f38c(Object *o);

void func_801b40c0_slot04_0b(Object *o, Object *p) {
    if ((s32)o == p->field_28) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}
