/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
int func_801b3a40_slot04_04(Object *obj);
void func_80131638(Object *object);
void func_801b39fc_slot04_04(Object *obj);

void func_801b386c_slot04_04(Object *obj) {
    if (func_801b3a40_slot04_04(obj) < 0 && obj->pos_y >= obj->field_70) {
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 2;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801307e0(obj, 0x1a);
    } else {
        if (((Slot04bObj *)obj)->field_3a != 0) {
            obj->field_07 = obj->field_07 + 1;
            func_801204f4(obj, obj->side, 0xc);
            func_80140770(obj, 4, 0xa, 0x12, 0, 0, 1);
        }
        func_80130efc(obj);
    }
}

void func_801b3934_slot04_04(Object *obj) {
    if (func_801b3a40_slot04_04(obj) < 0 && obj->pos_y > obj->field_70) {
        func_80131638(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3994_slot04_04(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_801204f4(obj, obj->side, 0xc);
    func_80140770(obj, 4, 0xa, 0xf, 0, 0, 1);
    func_801b39fc_slot04_04(obj);
}
