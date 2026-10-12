/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_801369b0(Cam *cam) {
    init_cam_position_and_consume_pending(cam, 0x1c0, 0);
}

void init_cam_position_and_consume_pending(Cam *cam, short x, short y) {
    init_cam_position(cam, x, y);
    if (cam->pending_flag != 0) {
        cam->pending_flag = 0;
        cam->field_04++;
    }
}
