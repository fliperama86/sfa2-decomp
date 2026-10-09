/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1210_slot04_03(Object *obj) {
    if (obj->field_240 == 0) {
        if ((s16)obj->field_c6 >= 0x30) {
            return 1;
        }
    }
    return 0;
}

