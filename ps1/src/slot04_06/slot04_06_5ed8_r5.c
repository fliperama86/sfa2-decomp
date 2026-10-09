/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b66fc_slot04_06(Object *obj);

void func_801b6440_slot04_06(Object *obj) {
    if ((func_801b66fc_slot04_06(obj) << 16) > 0) {
        obj->field_4c = 0x2000;
        if ((obj->field_0b ^ (obj->field_03 & 1)) != 0) {
            obj->field_4c = -0x2000;
        }
        obj->field_50 = 0x1c000;
        obj->field_58 = -0x1800;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        obj->field_05++;
    }
    func_80131094(obj);
}
