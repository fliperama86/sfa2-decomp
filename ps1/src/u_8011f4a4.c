/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


Pooled *func_8011f4a4(void) {
    Pooled *p;
    Pooled *result;
    short n = data_801ad348;
    if (n != 0) {
        data_801ad348 = n - 1;
        p = *--data_801a6980;
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
