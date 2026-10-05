/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80136898(Cam *cam, int delta) {
    cam->field_22 = cam->field_36 + delta;
}

void func_801368ac(Cam *cam, int delta) {
    cam->field_26 = cam->field_3a + delta;
}
