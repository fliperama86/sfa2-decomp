/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b5764_slot04_11(Object *obj);
void func_801b4970_slot04_11(Object *obj);

void func_801b45a8_slot04_11(Object *obj) {
    func_801b4970_slot04_11(obj);
    func_801b5764_slot04_11(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3 = 4;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801307e0(obj, 0x4c);
    } else if (obj->ground_y - 0x8a >= obj->pos_y) {
        obj->state_3++;
        func_801209c4(obj);
        func_801307e0(obj, 0x50);
    } else {
        update_sequence(obj);
    }
}
