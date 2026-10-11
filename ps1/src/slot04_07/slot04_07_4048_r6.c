/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4868_slot04_07(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        if (obj->field_0b == 0) {
            obj->pos_x = obj->pos_x - 0x38;
        } else {
            obj->pos_x = obj->pos_x + 0x38;
        }
    }
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    }
}
