/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1454_slot04_05[];

void func_801b1718_slot04_05(Object *obj) {
    if (obj->field_50 < 0) {
        obj->field_58 = obj->field_58 - data_801c1454_slot04_05[obj->field_12a >> 1];
    }
    if ((u8)func_80130184(obj) != 0) {
        if (obj->field_3a & 0x80) {
            return;
        }
    } else {
        obj->field_07++;
        func_801209c4(obj);
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
    }
    func_80130efc(obj);
}
