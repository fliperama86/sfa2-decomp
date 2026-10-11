/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2f24_slot04_0b[])(Object *, Object *);

void func_801b3d44_slot04_0b(Object *o, Object *p) {
    data_801c2f24_slot04_0b[o->state_1](o, p);
    func_8011ffdc(o);
}
