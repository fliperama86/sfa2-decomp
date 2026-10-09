/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c3d9c_slot04_08[];
extern u16 data_801c3d8c_slot04_08[];
u8 func_8013f8c4(Object *object, int a, int b);

void func_801b3a4c_slot04_08(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    obj->field_17b = 1;
    if (obj->field_49 != 0) {
        obj->field_225 = 1;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x39);
}

void func_801b3abc_slot04_08(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else if ((t & 0xff) && func_8013f8c4(obj, -0x30, data_801c3d9c_slot04_08[(obj->field_12a >> 1) * 2]) != 0) {
        obj->field_07++;
        obj->field_46 = data_801c3d8c_slot04_08[(obj->field_12a >> 1) * 2];
        func_80141f28(obj, 0x15);
        func_80120554(obj, obj->side ^ 1, 0x31a);
        func_801204f4(obj, obj->side, 0x12);
        func_801307e0(obj, 0x5d);
    } else {
        func_80130efc(obj);
    }
}
