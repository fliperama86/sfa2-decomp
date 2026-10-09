/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3c4c_slot04_08[];

u8 func_8013f8c4(Object *obj, int a, int b);

void func_801b1d74_slot04_08(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        obj->field_225 = 1;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x39);
}

void func_801b1df4_slot04_08(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else if (t & 0xff) {
        if (func_8013f8c4(obj, -0x30, *(s16 *)(data_801c3c4c_slot04_08 + (obj->field_12a & 0xfe))) != 0) {
            obj->field_07++;
            func_80120554(obj, obj->side ^ 1, 0x31a);
            func_801204f4(obj, obj->side, 0x12);
            func_80141f28(obj, 0x20);
            func_801307e0(obj, 0x1c);
        } else {
            func_80130efc(obj);
        }
    } else {
        func_80130efc(obj);
    }
}
