/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80027bc4_slot27[];
extern SequenceStep *data_80027c18_slot27[];
extern void (*data_80026ee8_slot27[])(Object *);

void func_80014010_slot27(Object *obj) {
    u8 *mode = &game_state.mode;
    int *side = data_80190464;
    Object *o = obj->other;

    if ((((*mode | game_state.field_07) >> (*side = o->side)) & 1) != 0) {
        obj->field_05 = 0;
        obj->field_04 = obj->field_04 + 1;
        func_80130768(obj, obj->field_48, data_80027bc4_slot27);
        obj->field_01 = 1;
        if (((*mode >> *side) & 1) != 0) {
            obj->field_05 = obj->field_05 + 1;
            func_80130768(obj, obj->field_48, data_80027c18_slot27);
        }
    }
}

void func_800140e0_slot27(Object *object) {
    data_80026ee8_slot27[object->field_05](object);
}
