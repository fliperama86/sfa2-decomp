/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4db8_slot04_0b(Object *obj, int index) {
    SequenceStep **table;
    if (((Slot04aObj *)ref_other.p)->side == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    func_80130768(obj, (s16)index, table);
}

void func_801b4e0c_slot04_0b(Object *obj, int dx, int dy) {
    s16 x = dx;
    if (obj->facing_side == 0) {
        x = -x;
    }
    obj->pos_x = x + ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y - dy;
}
