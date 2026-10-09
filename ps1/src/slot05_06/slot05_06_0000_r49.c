/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd4c8_slot05_06[];
extern u8 data_801dd4d0_slot05_06[];
extern ObjectFn data_801dd4d8_slot05_06[];

void func_801cc2d4_slot05_06(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    obj->field_159 = 0;
    obj->field_157 = 0;
    obj->field_29c = 3;
    func_80141f28(obj, 4);
    func_801307e0(obj, 0x2d);
    obj->field_38 = *(u16 *)(data_801dd4c8_slot05_06 + (obj->field_12a & 0xfe));
}

void func_801cc354_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a != 0) {
        ((Slot04aObj *)obj)->field_3a = 0;
        obj->field_38 = *(u16 *)(data_801dd4d0_slot05_06 + (obj->field_12a & 0xfe));
    }
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_801204f4(obj, obj->side, 1);
        func_801307e0(obj, (obj->field_12a >> 1) + 0x49);
    }
}

void func_801cc3f8_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_17b = 0;
        func_801312b8(obj);
    }
}

void func_801cc438_slot05_06(Object *obj) {
    data_801dd4d8_slot05_06[obj->field_07](obj);
}
