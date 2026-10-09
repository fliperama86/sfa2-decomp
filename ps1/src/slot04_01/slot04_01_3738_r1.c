/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b388c_slot04_01(Object *obj);

void func_801b3738_slot04_01(Object *obj) {
    if (func_801b388c_slot04_01(obj)) {
        if (obj->field_70 <= obj->pos_y) {
            obj->pos_y = obj->field_70;
            game_state.field_63 = 0x18;
        }
    }
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0x11, 0, 1, 0);
    }
    func_80130efc(obj);
}

void func_801b37d4_slot04_01(Object *obj) {
    if (func_801b388c_slot04_01(obj)) {
        if (obj->field_70 <= obj->pos_y) {
            obj->pos_y = obj->field_70;
            game_state.field_63 = 0x18;
            obj->field_07 = obj->field_07 + 1;
        }
    }
}

void func_801b383c_slot04_01(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    }
}
