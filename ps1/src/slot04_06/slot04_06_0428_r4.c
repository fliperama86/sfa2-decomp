/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);

void func_801b2dc0_slot04_06(Object *obj) {
    func_801b4850_slot04_06(obj);
    if (obj->pos_y < obj->ground_y) {
        func_801b4818_slot04_06(obj);
    } else {
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801312b8(obj);
    }
}
