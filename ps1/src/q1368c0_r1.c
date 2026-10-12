/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801368c0(Cam *cam) {
    cam->field_12 = cam->pos_x;
    cam->field_16 = cam->pos_y;
    cam->field_50 = cam->field_54;
    cam->field_50 += cam->field_48 * 2;
    cam->field_50 += cam->field_4a * 2;
}
