/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


Pooled *func_8011f430(void) {
    Pooled *p;
    Pooled *result;
    short n = data_801ac618;
    if (n != 0) {
        data_801ac618 = n - 1;
        data_801abefc++;
        p = *--data_801a4fe4;
        p->field_08 = 0;
        p->field_0a = 0;
        p->field_0c = 0;
        p->field_0e = 0;
        result = p;
    } else {
        result = 0;
    }
    return result;
}
