/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern SequenceStep **data_801c3ea4_slot04_08;
extern SequenceStep **data_801c3ea8_slot04_08;

void func_801b4ac0_slot04_08(Object *obj, Object *other) {
    u8 b;
    u16 h;

    obj->field_04++;
    obj->field_1c = other->field_1c;
    obj->field_09 = 3;
    obj->field_0c = other->field_0c;
    obj->field_0d = other->field_0d;
    obj->field_0e = other->field_0e;
    b = other->field_0b;
    obj->field_48 = 0;
    obj->field_46 = 0;
    obj->field_0b = b;
    ref_other.p = obj->field_3c;
    h = ref_other.p->field_1c;
    obj->field_81 = 6;
    obj->field_1c = h;
    if (other->side == 0) {
        data_801c3ea4_slot04_08 = data_1f8000b4;
    } else {
        data_801c3ea8_slot04_08 = data_1f800164;
    }
    if (other->side == 0) {
        func_80130768(obj, (u8)(obj->field_03 + 8), data_801c3ea4_slot04_08);
    } else {
        func_80130768(obj, (u8)(obj->field_03 + 8), data_801c3ea8_slot04_08);
    }
    obj->field_01 = 1;
}
