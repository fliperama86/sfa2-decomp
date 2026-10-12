/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_timer_or_record_hit_count_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = ((Object *)obj)->hit_count_3;
        }
    } else if (obj->timer != 0) {
        obj->timer -= 1;
    }
    push_other_by_vel_x(o);
}
