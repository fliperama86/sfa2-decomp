/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


Pooled *alloc_pooled_b(void) {
    Pooled *p;
    Pooled *result;
    short n = data_801ad348;
    if (n != 0) {
        data_801ad348 = n - 1;
        p = *--data_801a6980;
        p->rect_x = 0;
        p->rect_y = 0;
        p->field_0c = 0;
        p->field_0e = 0;
        result = p;
    } else {
        result = 0;
    }
    return result;
}
