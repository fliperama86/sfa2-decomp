/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bf120_slot04_01[];

void func_801b3640_slot04_01(Object *obj) {
    int i = 0;

    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, ((Slot04aObj *)obj)->field_a6 ^ 1, 0x31a);
    func_801204f4(obj, ((Slot04aObj *)obj)->field_a6, 6);
    if (obj->field_cd == 0) {
        i = obj->field_c2 >> 15;
    }
    obj->field_4c = data_801bf120_slot04_01[i];
    obj->field_0b = i;
    obj->field_50 = 0x20000;
    obj->field_58 = -0x2000;
    func_801307e0(obj, 0x19);
}
