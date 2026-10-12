/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1340_slot04_08(Object *obj) {
    if ((s16)obj->meter >= 0x30) {
        obj->substate_index = 0;
        return 1;
    }
    return 0;
}
