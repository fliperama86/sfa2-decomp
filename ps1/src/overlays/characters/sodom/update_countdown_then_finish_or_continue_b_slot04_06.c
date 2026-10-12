/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_timer_or_record_hit_count_slot04_06(Object *obj);

void update_countdown_then_finish_or_continue_b_slot04_06(Object *obj) {
    obj->hit_count_3++;
    update_sequence(obj);
    ((Slot04bObj *)obj)->countdown--;
    if (((Slot04bObj *)obj)->countdown != 0) {
        update_timer_or_record_hit_count_slot04_06(obj);
    } else {
        if (obj->active_state != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 3;
            obj->state_3 = 1;
        }
        init_object_after_hit_and_spawn_three_effects(obj);
    }
}
