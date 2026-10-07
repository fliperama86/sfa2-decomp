/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801bfe9c_slot04_00[];
void func_801483a4(Object *object, int a_arg, int b_arg);
void func_80148774(Object *object);
void func_80145d20(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);

void func_801b23cc_slot04_00(Object *obj) {
    obj->field_07++;
    obj->field_50 = 0x40000;
    obj->field_58 = -0x6000;
    obj->field_4c = 0;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x28);
}

void func_801b2438_slot04_00(Object *obj) {
    if ((u8)obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_80148774(obj);
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0x28, 0x3c);
    }
    func_80130efc(obj);
}

void func_801b24c0_slot04_00(Object *obj) {
    int i = 0;

    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        obj->field_07++;
        func_801204f4(obj, obj->side, 9);
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801bfe9c_slot04_00[i];
    }
}
