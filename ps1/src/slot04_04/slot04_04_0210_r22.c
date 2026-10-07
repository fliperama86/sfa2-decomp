/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c45a8_slot04_04[];

void func_801b3e74_slot04_04(Object *o) {
    data_801c45a8_slot04_04[o->field_05](o);
    func_8011ffdc(o);
}
