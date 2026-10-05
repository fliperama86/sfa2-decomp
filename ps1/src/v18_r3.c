/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011f14c(Slab172 *o) {
    func_80119b3c(o);
    func_8011f404(o, 0xc0);
    o->field_74 = 0;
    o->field_67 = 0;
    o->field_44 = 0;
    o->field_08 = 8;
    data_801a6960 = data_801a6960 + 1;
    o->field_26 = 0;
    o->field_28 = 0;
    o->field_2c = 0;
    o->field_30 = 0;
    o->field_34 = 0;
    o->field_90 = 0;
    data_801a89b0[data_801a6960] = o;
}
