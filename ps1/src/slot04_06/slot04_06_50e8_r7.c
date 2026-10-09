/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5d68_slot04_06(Object *obj, int idx) {
    SequenceStep **table;

    obj->field_48 = idx;
    if (obj->field_66 == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    func_80130768(obj, (u8)idx, table);
}
