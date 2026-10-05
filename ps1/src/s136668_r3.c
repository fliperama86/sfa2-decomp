/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_801369b0(Cam *cam) {
    func_801369d4(cam, 0x1c0, 0);
}

void func_801369d4(Cam *cam, short x, short y) {
    func_80136a2c(cam, x, y);
    if (cam->field_01 != 0) {
        cam->field_01 = 0;
        cam->field_04++;
    }
}
