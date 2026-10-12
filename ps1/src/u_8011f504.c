/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011f504(Pooled *p) {
    Pooled **sp;
    sp = data_801a4fe4;
    data_801ac618++;
    data_801abefc--;
    data_801a4fe4 = sp + 1;
    *sp = p;
    p->active_flag = 0;
    p->rect_x = 0;
    p->rect_y = 0;
    p->field_0c = 0;
    p->field_0e = 0;
}
