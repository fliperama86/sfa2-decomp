/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b56e0_slot04_17(Object *obj);
void func_801b48fc_slot04_17(Object *obj);

void func_801b4534_slot04_17(Object *obj) {
    func_801b48fc_slot04_17(obj);
    func_801b56e0_slot04_17(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3 = 4;
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
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
