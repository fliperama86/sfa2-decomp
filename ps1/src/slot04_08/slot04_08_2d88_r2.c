/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2eb4_slot04_08(Object *obj) {
    if (((Slot04aObj *)obj)->field_1c4 != 2) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        obj->field_4c = obj->field_4c + obj->field_54;
    }
    if ((s16)obj->field_3a & 0x8000) {
        ((Slot04aObj *)obj)->field_1c4++;
        if (((Slot04aObj *)obj)->field_1c4 == 3) {
            func_801312b8(obj);
        } else {
            obj->field_07 = 1;
            func_801307e0(obj, (obj->field_12a >> 1) + 0x43);
        }
    } else {
        func_80130efc(obj);
    }
}
