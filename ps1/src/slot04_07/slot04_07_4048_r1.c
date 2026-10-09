/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130efc(Object *object);
void func_80131638(Object *object);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
int func_801b41c4_slot04_07(Object *obj);
void func_801b40c8_slot04_07(Object *obj);

void func_801b4048_slot04_07(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a == 0) {
        func_80130efc(obj);
    } else {
        obj->field_50 = 0x40000;
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, -0x200, 0, 1, 0);
        func_801b40c8_slot04_07(obj);
    }
}

void func_801b40c8_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) >= 0 || obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        func_80131638(obj);
    }
}
