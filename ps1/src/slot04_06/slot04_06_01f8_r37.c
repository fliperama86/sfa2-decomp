/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c56c4_slot04_06[];

void func_801b63ec_slot04_06(Object *o) {
    data_801c56c4_slot04_06[o->field_05](o);
    func_8011ffdc(o);
}
