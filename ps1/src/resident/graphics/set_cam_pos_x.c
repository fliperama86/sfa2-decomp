/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_cam_pos_x(Cam *cam, int delta) {
    cam->pos_x = cam->base_x + delta;
}

void set_cam_pos_y(Cam *cam, int delta) {
    cam->pos_y = cam->base_y + delta;
}
