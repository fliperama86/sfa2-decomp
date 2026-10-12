/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3f30_slot04_0a(Object *obj);

void func_801b252c_slot04_0a(Object *obj) {
    s16 t;
    u16 u;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff00) == 0x100) {
        obj->step_flags = t & 0xff;
        obj->field_0b = obj->field_158;
    }
    u = obj->step_flags;
    if ((u & 0xff) != 0) {
        obj->step_flags = u & 0xff00;
        func_801b3f30_slot04_0a(obj);
    }
    if ((s16)obj->step_flags & 0x8000) {
        obj->pos_y_frac = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801312b8(obj);
    }
}
