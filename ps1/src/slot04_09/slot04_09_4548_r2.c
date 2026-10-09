/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4688_slot04_09(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    obj->field_157 = 0;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    func_801204f4(obj, obj->side, 0xd);
    func_801307e0(obj, 0x34);
}
