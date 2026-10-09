/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4530_slot04_04[];

void func_801b3c5c_slot04_04(Object *o) {
    data_801c4530_slot04_04[o->field_05](o);
    func_8011ffdc(o);
}
