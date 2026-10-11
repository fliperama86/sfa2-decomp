/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b2074_slot04_07(Object *obj) {
    int y;
    func_801b41c4_slot04_07(obj);
    y = obj->field_70;
    if (y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->pos_y = y;
        obj->state_3 = 5;
        obj->field_45 = 0;
        obj->field_17b = 0;
        func_801209c4(obj);
        func_801307e0(obj, 0x3b);
    }
}
