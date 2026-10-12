/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003a190_slot28[])(Object *);

void call_handler_by_state_1_9_slot28(Object *object) {
    data_8003a190_slot28[object->state_1](object);
}

void count_down_then_advance_state_1_2_slot28(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t == 0) {
        obj->state_1++;
    }
}
