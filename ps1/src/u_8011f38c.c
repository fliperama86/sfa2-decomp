/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011f38c(Object *o) {
    func_80119b3c(o);
    func_8011f404(o, 0xac);
    o->field_44 = 0;
    o->field_08 = 0x10;
    data_801a4fec = data_801a4fec + 1;
    o->field_26 = 0;
    data_801a68f0[data_801a4fec] = o;
}
