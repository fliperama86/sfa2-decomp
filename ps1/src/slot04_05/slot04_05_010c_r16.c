/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3cf8_slot04_05(Object *o, Object *p) {
    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    func_8011f38c(o);
}
