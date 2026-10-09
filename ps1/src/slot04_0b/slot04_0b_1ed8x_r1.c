/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c25b4_slot04_0b[];

void func_801b1ed8_slot04_0b(Object *o) {
    int k;
    int i;

    k = 0x20;
    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 6);
    func_80138ae8(&game_state, o);
    func_801204f4(o, o->side, 0xf);
    func_801204f4(o, o->side, 6);
    o->field_58 = 0xfffe6000;
    o->field_54 = 0;
    o->field_50 = 0xa0000;
    i = data_801c25b4_slot04_0b[o->field_12a >> 1];
    if (o->field_0b == 0) {
        o->field_4c = -i;
    } else {
        o->field_4c = i;
    }
    if (o->field_49 != 0) {
        k = 0x36;
    }
    func_801307e0(o, (o->field_12a >> 1) + k);
}
