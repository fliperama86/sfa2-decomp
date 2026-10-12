/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b2074_slot04_07(Object *obj) {
    int y;
    func_801b41c4_slot04_07(obj);
    y = obj->ground_y;
    if (y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->pos_y = y;
        obj->state_3 = 5;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        func_801209c4(obj);
        func_801307e0(obj, 0x3b);
    }
}
