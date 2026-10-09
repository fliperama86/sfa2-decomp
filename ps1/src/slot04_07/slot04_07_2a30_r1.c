/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1f94_slot04_07[];
extern Slot04_07Rec1fc4 data_801c1fc4_slot04_07[];

void func_801b2a30_slot04_07(Object *obj) {
    s32 *p;
    s32 a;
    s32 b;

    obj->field_17b = 1;
    obj->field_07++;
    func_801204f4(obj, obj->side, 6);
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    p = &data_801c1f94_slot04_07[(obj->field_12a >> 1) * 4];
    a = *p++;
    b = *p++;
    obj->field_50 = p[0];
    obj->field_58 = p[1];
    if (obj->field_0b == 0) {
        obj->field_4c = -a;
        obj->field_54 = -b;
    } else {
        obj->field_4c = a;
        obj->field_54 = b;
    }
    a = 0;
    if (obj->field_49 != 0) {
        a = 6;
    }
    a += obj->field_12a;
    func_801307e0(obj, data_801c1fc4_slot04_07[a >> 1].value);
}
