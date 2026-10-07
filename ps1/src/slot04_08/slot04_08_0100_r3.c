/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1340_slot04_08(Object *obj) {
    if ((s16)obj->field_c6 >= 0x30) {
        obj->field_12c = 0;
        return 1;
    }
    return 0;
}
