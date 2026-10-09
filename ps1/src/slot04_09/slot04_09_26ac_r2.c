/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);

void func_801b27f8_slot04_09(Object *obj) {
    if (obj->field_4c >= 0) {
        if (obj->field_0b == 0) {
            obj->field_4c = 0;
            obj->field_54 = 0;
        }
    } else {
        if (obj->field_0b != 0) {
            obj->field_4c = 0;
            obj->field_54 = 0;
        }
    }
    func_801b2858_slot04_09(obj);
}
