/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f38c(Object *o);

void func_801b4390_slot04_07(Object *o, Object *p) {
    if ((s32)o == p->field_28) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}
