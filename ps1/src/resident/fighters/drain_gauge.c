/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


int drain_gauge(Object *object) {
    int v;
    if (test_random_bit_in_table(object, table_8017d598)) {
        v = object->gauge - 1;
        object->gauge = v;
        if ((s16)v >= 0) {
            v = v - 1;
            object->gauge = v;
        }
    } else {
        v = object->gauge - 1;
        object->gauge = v;
    }
    if ((s16)v < 0) {
        object->gauge = 0;
        return 1;
    }
    return 0;
}
