/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801428e4(Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a_arg, int b_arg);
void func_80138b38(GameState *state, Object *object);

void func_801b2970_slot04_0a(Object *obj) {
    int a;
    int b;
    obj->field_225 = 1;
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    a = 0x80000;
    b = -0x4000;
    obj->field_45 = 1;
    obj->field_165 = 0;
    obj->field_50 = 0x28000;
    obj->field_58 = b;
    if (obj->field_0b == 0) {
        a = 0xfff80000;
        b = 0x4000;
    }
    obj->field_4c = a;
    obj->field_54 = b;
    func_80120554(obj, obj->side, 0x31c);
    func_801483a4(obj, -0x19, 0x44);
    func_80145d20(obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x30);
}
