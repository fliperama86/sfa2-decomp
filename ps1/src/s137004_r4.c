/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_side_flag(Object *object) {
    data_8018daf4[object->side] = 1;
}

void call_side_func_then_queue_job(Object *object) {
    if (object->side == 0) {
        store_player_left_image_rows_from_field();
    } else {
        store_player_right_image_rows_from_field();
    }
    queue_image_upload_job(0, 6);
}
