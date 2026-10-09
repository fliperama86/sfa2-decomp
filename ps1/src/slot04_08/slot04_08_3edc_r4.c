/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c3e9c_slot04_08;
extern SequenceStep **data_801c3ea0_slot04_08;
void func_801b44a8_slot04_08(Object *obj, Object *other);

void func_801b43fc_slot04_08(Object *obj, Object *other) {
    obj->field_04++;
    obj->field_1c = other->field_1c;
    obj->field_03 = other->kind;
    obj->field_0c = other->field_0c;
    obj->field_0d = other->field_0d;
    obj->field_0e = other->field_0e;
    obj->field_48 = 0;
    if (other->side == 0) {
        data_801c3e9c_slot04_08 = data_1f8000b4;
    } else {
        data_801c3ea0_slot04_08 = data_1f800164;
    }
    func_801b44a8_slot04_08(obj, other);
}
