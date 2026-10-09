/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9b24_slot06_0f(Object *obj);

void func_801e99c8_slot06_0f(Object *obj) {
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_0f = 1;
    obj->field_81 = 4;
    obj->field_04++;
    obj->pos_y += 0x58;
    if (obj->field_0b == 0) {
        obj->field_0a = 1;
    }
    func_801e9b24_slot06_0f(obj);
}
