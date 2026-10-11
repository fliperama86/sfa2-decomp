/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b0fec_slot04_07(Object *obj) {
    int limit;

    if (func_801b41c4_slot04_07(obj) >= 0) {
        update_sequence(obj);
    } else {
        limit = obj->field_70;
        if (limit > obj->pos_y) {
            update_sequence(obj);
        } else {
            obj->state_3 = 1;
            obj->pos_y = limit;
            obj->pos_y_frac = 0;
            obj->field_45 = 0;
            func_801209c4(obj);
            func_801307e0(obj, 0x2e);
        }
    }
}
