/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_80130768(Object *object, int index, SequenceStep **table);

/* func_80130768 is declared here with an int second parameter, not the s16
   of protos.h, which is why this unit does not include protos.h. */
void func_801b7214_slot04_11(Object *obj, int idx) {
    SequenceStep **table;

    if (obj->field_66 == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    obj->field_48 = idx;
    func_80130768(obj, idx, table);
}
