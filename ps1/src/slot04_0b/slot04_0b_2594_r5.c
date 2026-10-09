/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2b50_slot04_0b(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x2c);
}

void func_801b2bac_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80148494(obj, -0x10, 0x47);
        func_80120554(obj, obj->side, 0x31c);
    }
    func_80130efc(obj);
}

void func_801b2c30_slot04_0b(Object *obj) {
    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        obj->field_165 = 0;
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xf);
        func_801204f4(obj, obj->side, 4);
        obj->field_27b = 3;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
    }
}
