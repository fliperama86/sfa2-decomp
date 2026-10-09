/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80141e5c(Object *object);

void func_801b4e3c_slot04_09(Object *obj, Object *unused) {
    obj->field_07++;
    if (obj->field_246 == 0) {
        func_80141f28(obj, 3);
    }
    func_80120554(obj, obj->side ^ 1, 0x31b);
    func_801307e0(obj, 0x18);
    func_80141e5c(obj);
}
