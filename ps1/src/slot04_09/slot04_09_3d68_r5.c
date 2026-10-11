/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);
void func_801b5478_slot04_09(Object *obj);

void func_801b41c0_slot04_09(Object *obj) {
    func_801b2858_slot04_09(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        func_801b5478_slot04_09(obj);
    }
    update_sequence(obj);
}
