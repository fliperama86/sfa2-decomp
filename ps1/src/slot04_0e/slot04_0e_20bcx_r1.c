/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_801b2658_slot04_0e(Object *obj);
int func_801b20bc_slot04_0e(Object *obj);

int func_801b20bc_slot04_0e(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->field_15a = 3;
    obj->field_159 = 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_48 = 0xff;
    if (!(obj->field_130 & 0x2000)) {
        obj->field_48 = 1;
    }
    func_801b2658_slot04_0e(obj);
    return 1;
}
