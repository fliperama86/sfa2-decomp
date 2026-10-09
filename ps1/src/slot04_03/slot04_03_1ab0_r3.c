/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1cfc_slot04_03(Object *obj) {
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    obj->field_4c = 0x60000;
    obj->field_54 = -0x8000;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x32);
}

void func_801b1d70_slot04_03(Object *obj) {
    if (obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x18, 0x28);
    }
    func_80130efc(obj);
}
