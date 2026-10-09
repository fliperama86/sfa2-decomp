/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_80130768(Object *object, int index, SequenceStep **table);

void func_801b6adc_slot04_17(Object *obj, int idx);

void func_801b6adc_slot04_17(Object *obj, int idx) {
    SequenceStep **table;

    if (obj->field_66 == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    obj->field_48 = idx;
    func_80130768(obj, idx, table);
}
