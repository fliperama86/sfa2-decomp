/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *object);

void func_801b2a1c_slot04_0b(Object *obj) {
    if (func_801b2510_slot04_0b(obj)) {
        update_sequence(obj);
    } else {
        if (obj->direction_index == 4 && obj->variant_index_2 == 0) {
            obj->state_3 = 6;
            obj->variant_index_2 = 3;
            func_801307e0(obj, 0x48);
        } else {
            obj->state_3++;
            obj->active_state = 0;
            obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
            func_801209c4(obj);
            func_801307e0(obj, 0x2a);
        }
    }
}
