/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_direction_index_slot04_07(Object *obj);
void update_sequence_with_slide_step_slot04_07(Object *obj);

void count_down_then_dispatch_by_pressed_bits_slot04_07(Object *obj) {
    s16 t = obj->countdown;

    if (t != 0) {
        t -= 1;
        obj->countdown = t;
        if (obj->pressed_bits & 0x68) {
            call_handler_by_direction_index_slot04_07(obj);
        } else {
            update_sequence_with_slide_step_slot04_07(obj);
        }
    } else {
        update_sequence_with_slide_step_slot04_07(obj);
    }
}
