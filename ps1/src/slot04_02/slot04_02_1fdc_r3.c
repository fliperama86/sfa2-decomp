/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6334_slot04_02[];

void func_801b2310_slot04_02(Object *o) {
    s32 t;
    u16 a;

    o->field_07++;
    func_80141f28(o, 6);
    func_80138ae8(&game_state, o);
    o->field_58 = -0x3000;
    t = data_801c6334_slot04_02[o->field_12a >> 1];
    if (o->field_0b != 0) {
        if (o->field_4c < 0) {
            t = -t;
        }
    } else {
        if (o->field_4c <= 0) {
            t = -t;
        }
    }
    o->field_4c += t;
    func_80120554(o, o->side, 0x320);
    a = 0x23;
    if (o->field_12a != 0) {
        ((Slot04aObj *)o)->field_1a4 = o->field_12a;
    } else {
        ((Slot04aObj *)o)->field_1a4 = 1;
    }
    a += o->field_12a >> 1;
    func_801307e0(o, a);
}
