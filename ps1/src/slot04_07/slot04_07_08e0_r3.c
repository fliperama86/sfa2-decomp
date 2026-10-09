/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0acc_slot04_07(Object *obj) {
    func_80130efc(obj);
    if (obj->field_3a & 0xff) {
        if (obj->field_0b == 0) {
            obj->pos_x -= 0x1c;
        } else {
            obj->pos_x += 0x1c;
        }
    }
    if ((s16)obj->field_3a & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        func_801312b8(obj);
    }
}
