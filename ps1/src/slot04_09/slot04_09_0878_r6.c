/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b44c4_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->variant_index_2 = 0;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
