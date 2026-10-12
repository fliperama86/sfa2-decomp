/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80136898(Cam *cam, int delta) {
    cam->pos_x = cam->base_x + delta;
}

void func_801368ac(Cam *cam, int delta) {
    cam->pos_y = cam->base_y + delta;
}
