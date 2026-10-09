/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a_arg, int b_arg);

void func_801b21c8_slot04_05(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    if (obj->field_0b != 0) {
        obj->field_4c = 0x50000;
    } else {
        obj->field_4c = -0x50000;
    }
    obj->field_54 = 0;
    obj->field_50 = 0x88000;
    obj->field_58 = -0x9000;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x29);
}

void func_801b2260_slot04_05(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x10, 0x43);
    }
    func_80130efc(obj);
}
