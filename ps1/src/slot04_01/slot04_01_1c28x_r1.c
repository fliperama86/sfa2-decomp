/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801befc4_slot04_01[];

void func_801b1c28_slot04_01(Object *o) {
    Slot04aObj *obj;
    int a3;
    s32 t;

    obj = (Slot04aObj *)o;
    t = o->field_07;
    t++;
    o->field_07 = t;
    obj->field_1a5 = 0;
    t = o->field_50;
    if (t >= 0) {
        func_801204f4(o, o->side, 0xc);
    } else if (o->pos_y - o->field_70 < -0x2f) {
        obj->field_1a5 = 1;
        func_801204f4(o, o->side, 0xc);
    }
    func_80141f28(o, 6);
    func_80138ae8(&game_state, o);
    o->field_58 = -0x3000;
    a3 = data_801befc4_slot04_01[o->field_12a >> 1];
    if (o->field_0b == 0) {
        if (o->field_4c <= 0) {
            a3 = -a3;
        }
    } else {
        if (o->field_4c < 0) {
            a3 = -a3;
        }
    }
    o->field_4c = a3 + o->field_4c;
    func_80120554(o, o->side, 0x320);
    a3 = o->field_12a >> 1;
    ((Slot04aObj *)o)->field_1a4 = a3 + 1;
    func_801307e0(o, a3 + 0x25);
}
