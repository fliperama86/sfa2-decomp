/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c3e9c_slot04_08;
extern SequenceStep **data_801c3ea0_slot04_08;
void func_801b45a4_slot04_08(Object *obj, Object *other);

void func_801b44a8_slot04_08(Object *obj, Object *other) {
    obj->field_01 = 0;
    if (other->kind == obj->field_03) {
        if (other->frame->field_09 != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&other->field_10;
            *(s32 *)&obj->field_14 = *(s32 *)&other->field_14;
            obj->field_0b = other->field_0b;
            obj->field_01 = 1;
            if (obj->field_48 != other->frame->field_09) {
                obj->field_48 = other->frame->field_09;
                if (other->side == 0) {
                    func_80130768(obj, obj->field_48, data_801c3e9c_slot04_08);
                } else {
                    func_80130768(obj, obj->field_48, data_801c3ea0_slot04_08);
                }
            } else {
                func_80131094(obj);
            }
        } else {
            obj->field_48 = 0;
        }
    } else {
        func_801b45a4_slot04_08(obj, other);
    }
}
