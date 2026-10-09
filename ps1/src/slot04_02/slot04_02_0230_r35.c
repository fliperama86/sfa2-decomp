/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c786c_slot04_02[];

void func_801b760c_slot04_02(Object *o) {
    data_801c786c_slot04_02[o->field_05](o);
    func_8011ffdc(o);
}
