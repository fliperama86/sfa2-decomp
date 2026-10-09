/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b223c_slot04_03(Object *obj);

extern u8 data_801c0674_slot04_03[];

void func_801b1df0_slot04_03(Object *obj) {
    u8 i;

    func_80130efc(obj);
    if ((s16)obj->field_3a & 0xff00) {
        i = 0;
        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c0674_slot04_03[i];
        func_801204f4(obj, obj->side, 5);
        func_801204f4(obj, obj->side, 0xd);
    }
    if ((u8)obj->field_3a == 0) {
        func_801b223c_slot04_03(obj);
    }
}
