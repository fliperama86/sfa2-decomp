/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b56e0_slot04_17(Object *obj);

void func_801b2730_slot04_17(Object *obj) {
    func_801b56e0_slot04_17(obj);
    if (obj->pos_y <= obj->field_70 - 0x8a) {
        obj->field_07++;
        func_801209c4(obj);
        func_801307e0(obj, 0x2d);
    } else {
        func_80130efc(obj);
    }
}
