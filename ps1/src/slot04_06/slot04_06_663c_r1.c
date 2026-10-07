/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b66fc_slot04_06(Object *obj);

void func_801b663c_slot04_06(Object *obj) {
    if ((func_801b66fc_slot04_06(obj) << 16) > 0) {
        ((Slot04aObj *)obj)->field_47 = 8;
        obj->field_38 = 1;
        obj->field_05++;
        obj->pos_y = obj->field_70;
        func_80131094(obj);
    }
}

void func_801b6698_slot04_06(Object *obj) {
    ((Slot04aObj *)obj)->field_47--;
    if (((Slot04aObj *)obj)->field_47 & 0x80) {
        obj->field_04++;
    }
}
