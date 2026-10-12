/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80032a5c_slot28[])(Object *);

void call_handler_by_state_1_slot28(Object *object) {
    data_80032a5c_slot28[object->state_1](object);
}

void update_countdown_with_decreasing_reload_slot28(Object *o) {
    ((Slot28Obj *)o)->field_01 = 0;
    if (((Slot28Obj *)o)->timer_reload != 0) {
        ((Slot28Obj *)o)->countdown_timer --;
        if (((Slot28Obj *)o)->countdown_timer == 0) {
            ((Slot28Obj *)o)->field_01 = 1;
            ((Slot28Obj *)o)->timer_reload --;
            ((Slot28Obj *)o)->countdown_timer = ((Slot28Obj *)o)->timer_reload;
        }
    } else {
        ((Slot28Obj *)o)->count ++;
    }
}

void set_object_flag_byte_to_one_slot28(Object *obj) {
    obj->field_01 = 1;
}
