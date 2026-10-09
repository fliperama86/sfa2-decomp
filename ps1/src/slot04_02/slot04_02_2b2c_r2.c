/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2d40_slot04_02(Object *obj) {
    obj->field_249 = 2;
    *(s32 *)&obj->field_4c = 0x80000;
    obj->field_54 = -0x8000;
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x2a);
}

void func_801b2db8_slot04_02(Object *obj) {
    obj->field_249 = 2;
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x12, 0x30);
    }
    func_80130efc(obj);
}
