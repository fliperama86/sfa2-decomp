/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b188c_slot04_09(Object *obj);

int func_801b1800_slot04_09(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->counter != 0) return 0;
    return func_801b188c_slot04_09(obj);
}

int func_801b1844_slot04_09(Object *obj) {
    if (obj->field_7e == 0) {
        if (obj->remaining_count == 0) {
            return 0;
        }
    }
    return func_801b188c_slot04_09(obj);
}
