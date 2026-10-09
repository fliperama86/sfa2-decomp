/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ebc1c_slot06_0c[];

void func_801e988c_slot06_0c(Slot06Pal *pal);

void func_801e970c_slot06_0c(Object *obj) {
    Slot06Pal *p0 = (Slot06Pal *)((u8 *)obj + 0x5c);
    Slot06Pal *p1 = (Slot06Pal *)((u8 *)obj + 0x70);
    Slot06Pal *p2 = (Slot06Pal *)((u8 *)obj + 0x4c);

    obj->field_01 = 0;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_81 = 4;
    obj->field_04++;
    p0->field_0c = 0xe;
    p1->field_0c = 0xf;
    p2->field_0c = 0x13;
    p0->field_08 = 0x1a0;
    p1->field_08 = 0x260;
    p2->field_08 = 0x200;
    p0->field_0a = 0x60;
    p1->field_0a = 0x60;
    p2->field_0a = 0x60;
    /* The listing stores field_04 twice: the incremented value above, then 1. */
    obj->field_04 = 1;
    func_801e988c_slot06_0c(p0);
    func_801e988c_slot06_0c(p1);
    func_801e988c_slot06_0c(p2);
    func_80130700(obj, data_801ebc1c_slot06_0c[1]);
}
