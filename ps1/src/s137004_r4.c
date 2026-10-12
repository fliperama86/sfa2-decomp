/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_side_flag(Object *object) {
    data_8018daf4[object->side] = 1;
}

void call_side_func_then_queue_job(Object *object) {
    if (object->side == 0) {
        func_80136e90();
    } else {
        func_80136ed0();
    }
    queue_image_upload_job(0, 6);
}
