/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_801411c4(Object *object) {
    int v;
    if (func_8014a170(object, table_8017d598)) {
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
