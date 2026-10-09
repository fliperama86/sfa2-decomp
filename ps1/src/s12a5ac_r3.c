/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8014a058(Object *object);
int func_8014a0fc(Object *object);

void func_8012ab5c(Object *object) {
    if (func_80149c30(object)) {
        func_80130304(object);
    } else if ((u8)func_8014a058(object)) {
        func_8012f384(object);
    } else if ((u8)func_8014a0fc(object)) {
        func_8012f3e0(object);
    } else {
        func_80130efc(object);
    }
}
