/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a_arg, int b_arg);
extern u8 data_801c2608_slot04_0b[];

void func_801b2594_slot04_0b(Object *obj) {
    obj->field_58 = -0x3000;
    obj->field_50 = 0x20000;
    obj->field_54 = 0;
    obj->field_07++;
    if (obj->field_0b == 0) {
        obj->field_4c = -0x80000;
    } else {
        obj->field_4c = 0x80000;
    }
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x3f);
}

void func_801b2624_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_801483a4(obj, -0x28, 0x62);
        func_80120554(obj, obj->side, 0x31c);
        func_801204f4(obj, obj->side, 7);
    }
    func_80130efc(obj);
}

void func_801b26b8_slot04_0b(Object *obj) {
    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        obj->field_165 = 0;
        obj->field_07++;
        obj->field_27b = data_801c2608_slot04_0b[obj->field_12a >> 1];
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
    }
}
