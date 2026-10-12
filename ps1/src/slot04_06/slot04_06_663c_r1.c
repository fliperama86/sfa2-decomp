/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b66fc_slot04_06(Object *obj);

void func_801b663c_slot04_06(Object *obj) {
    if ((func_801b66fc_slot04_06(obj) << 16) > 0) {
        ((Slot04aObj *)obj)->timer = 8;
        obj->step_timer = 1;
        obj->state_1++;
        obj->pos_y = obj->ground_y;
        advance_sequence_checked(obj);
    }
}

void func_801b6698_slot04_06(Object *obj) {
    ((Slot04aObj *)obj)->timer--;
    if (((Slot04aObj *)obj)->timer & 0x80) {
        obj->state_0++;
    }
}
