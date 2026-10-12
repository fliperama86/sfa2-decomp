/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_2_when_variant_index_set_2_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->variant_index = 0;
        obj->countdown = 0x30;
        obj->state_1 = obj->state_1 + 1;
        set_sequence_from_table_5(obj, 2, ((Slot28Obj *)obj)->sequence_ptr);
    }
}

void shake_y_alternating_until_countdown_slot28(Object *obj) {
    int d;
    if (((Slot28Obj *)obj)->timer_reload != 0) {
        d = -1;
    } else {
        d = 1;
    }
    ((Slot28Obj *)obj)->pos_y = ((Slot28Obj *)obj)->pos_y - d;
    ((Slot28Obj *)obj)->countdown_timer--;
    ((Slot28Obj *)obj)->timer_reload = ((Slot28Obj *)obj)->timer_reload ^ 1;
    if (((Slot28Obj *)obj)->countdown_timer == 0) {
        obj->state_1 = obj->state_1 + 1;
    }
}
