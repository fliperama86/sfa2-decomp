/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b359c_slot04_02(Object *obj) {
    if (obj->field_49 == 0) {
        store_image_rows_by_mode_and_queue_job(obj, 1);
    }
}
