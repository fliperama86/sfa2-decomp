/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2d6c_slot04_09(Object *obj) {
    Object *p = obj->other;

    p->field_0c = ((Slot04aObj *)obj)->field_334;
    p->image_row = ((Slot04aObj *)obj)->field_335;
    if (game_state.frame_counter & 2) {
        p->field_0c = 0xff;
        p->image_row = p->image_row + 3;
    }
}
