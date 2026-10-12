/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void save_cam_prev_pos_and_set_table_ptr(Cam *cam) {
    cam->prev_pos_x = cam->pos_x;
    cam->prev_pos_y = cam->pos_y;
    cam->table_ptr = cam->base_ptr;
    cam->table_ptr += cam->offset_a * 2;
    cam->table_ptr += cam->offset_b * 2;
}
