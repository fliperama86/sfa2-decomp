/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b388c_slot04_01(Object *obj);

void func_801b3918_slot04_01(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_46 = 0xa;
    func_80141f28(obj, 4);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801204f4(obj, obj->side, 6);
    if (obj->field_4c != 0) {
        obj->field_0b = (u32)obj->field_4c >> 31;
    }
    func_801307e0(obj, 0x1a);
}

void func_801b399c_slot04_01(Object *obj) {
    s16 t = obj->field_46;
    t -= 1;
    obj->field_46 = t;
    if (t == 0) {
        obj->field_07 = obj->field_07 + 1;
    }
    func_80130efc(obj);
}

void func_801b39e8_slot04_01(Object *obj) {
    func_801b388c_slot04_01(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->pos_y = obj->field_70;
        game_state.field_63 = 0x18;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b == 0) {
            obj->field_4c = 0x60000;
        } else {
            obj->field_4c = -0x60000;
        }
    }
    func_80130efc(obj);
}
