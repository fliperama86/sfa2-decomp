/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b2048_slot04_0a(Object *obj);

void func_801b1f54_slot04_0a(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        func_80130efc(obj);
        return;
    }
    func_801b2048_slot04_0a(obj);
    if (func_801b1e04_slot04_0a(obj)) {
        obj->field_07++;
        func_80120554(obj, obj->side, 0x314);
        obj->pos_y = (u16)obj->field_70;
        obj->field_14 = 0;
        obj->field_17b = 0;
        func_801209c4(obj);
        func_801307e0(obj, 0x48);
        return;
    }
    func_80130efc(obj);
}
