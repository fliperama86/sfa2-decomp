/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80141248(Object *object) {
    if ((ref_other.p->pressed_bits & 0xfc) == 0) {
        object->gauge = object->gauge + 1;
    } else {
        int t = object->gauge - 1;
        object->gauge = t;
        if ((s16)t < 0) object->gauge = 0;
    }
}
