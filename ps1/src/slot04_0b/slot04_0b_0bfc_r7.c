/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);
void func_801204f4(Object *o, int b, int c);

extern s32 data_801c2568_slot04_0b[];

void func_801b15d0_slot04_0b(Object *obj) {
    int a;

    a = 0x26;
    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 0xf);
    func_801204f4(obj, obj->side, 5);
    obj->field_54 = -0x8000;
    obj->field_58 = -0x6000;
    obj->field_4c = data_801c2568_slot04_0b[obj->field_12a];
    obj->field_50 = data_801c2568_slot04_0b[obj->field_12a + 1];
    if (obj->field_49 != 0) {
        a = 0x33;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
