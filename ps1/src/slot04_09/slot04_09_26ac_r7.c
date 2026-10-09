/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2d6c_slot04_09(Object *obj) {
    Object *p = obj->other;

    p->field_0c = ((Slot04aObj *)obj)->field_334;
    p->field_0d = ((Slot04aObj *)obj)->field_335;
    if (game_state.field_1d & 2) {
        p->field_0c = 0xff;
        p->field_0d = p->field_0d + 3;
    }
}
