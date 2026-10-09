/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80021270_slot28(Object *obj) {
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04++;
    func_80130768(obj, obj->field_03, ((Slot28Obj *)obj)->field_6c);
}
