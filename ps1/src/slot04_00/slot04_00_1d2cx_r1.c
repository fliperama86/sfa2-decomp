/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bfe60_slot04_00[];
void func_80138ae8(GameState *state, Object *object);

void func_801b1d2c_slot04_00(Object *obj) {
    s32 a;
    s32 t;

    t = obj->field_07;
    t++;
    obj->field_07 = t;
    ((Slot04aObj *)obj)->field_1a5 = 0;
    t = obj->field_50;
    if (t >= 0) {
        func_801204f4(obj, obj->side, 9);
    } else if (obj->pos_y - obj->field_70 < -0x2f) {
        ((Slot04aObj *)obj)->field_1a5 = 1;
        func_801204f4(obj, obj->side, 9);
    }
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    obj->field_58 = -0x3000;
    a = data_801bfe60_slot04_00[obj->field_12a >> 1];
    if (obj->field_0b == 0) {
        if (obj->field_4c <= 0) {
            a = -a;
        }
    } else {
        if (obj->field_4c < 0) {
            a = -a;
        }
    }
    obj->field_4c = a + obj->field_4c;
    func_80120554(obj, obj->side, 0x320);
    a = obj->field_12a >> 1;
    ((Slot04aObj *)obj)->field_1a4 = a;
    func_801307e0(obj, a + 0x24);
}
