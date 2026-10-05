/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011f55c(Pooled *p) {
    Pooled **sp;
    data_801ad348++;
    sp = data_801a6980;
    data_801a6980 = sp + 1;
    *sp = p;
    p->field_00 = 0;
    p->field_08 = 0;
    p->field_0a = 0;
    p->field_0c = 0;
    p->field_0e = 0;
}
