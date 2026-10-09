/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80120554(Object *o, int b, unsigned c);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80131638(Object *object);
void func_801b3e00_slot04_0a(Object *object);

void func_801b3c74_slot04_0a(Object *obj) {
    obj->field_07++;
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_50 = 0;
    func_80141f28(obj, 4);
    func_801307e0(obj, 0x53);
}

void func_801b3cd0_slot04_0a(Object *obj) {
    int v;
    if ((u8)obj->field_3a != 2) {
        func_801b3e00_slot04_0a(obj);
    }
    if ((u8)obj->field_3a == 1) {
        func_801204f4(obj, obj->side, 8);
        obj->field_07++;
        if (obj->other->field_15b != 0) {
            func_80140770(obj, 0x24, 0xd, 0x11, 0, 0, 0);
        } else {
            func_80140770(obj, 0x24, 0xa, 0x11, 0, 0, 0);
        }
        if (obj->field_0b != 0) {
            obj->field_4c = 0xffff0000;
            v = 0x500;
        } else {
            obj->field_4c = 0x10000;
            v = -0x500;
        }
        obj->field_54 = v;
        obj->field_50 = 0;
    }
    func_80130efc(obj);
}

void func_801b3da8_slot04_0a(Object *obj) {
    func_801b3e00_slot04_0a(obj);
    if (obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        func_80131638(obj);
    }
}
