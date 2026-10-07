/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *obj);

void func_801b3024_slot04_0b(Object *obj) {
    if ((u8)func_801b2510_slot04_0b(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x2a);
    }
}
