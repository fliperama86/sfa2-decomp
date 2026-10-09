/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *object);

void func_801b2738_slot04_0b(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a == 2) {
        obj->field_45 = 1;
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xf);
        func_801204f4(obj, obj->side, 8);
        if (obj->field_0b == 0) {
            obj->pos_x -= 0x28;
        } else {
            obj->pos_x += 0x28;
        }
    }
}

void func_801b27cc_slot04_0b(Object *obj) {
    func_801b2510_slot04_0b(obj);
    if (obj->field_50 < 0) {
        obj->field_58 = -0x8000;
        obj->field_07++;
        obj->field_4c = 0;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x42);
    } else {
        func_80130efc(obj);
    }
}
