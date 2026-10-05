/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80136b34(Cam *cam) {
    cam->field_04++;
    cam->field_70 = 0;
    func_80136b60(cam);
}

void func_80136b60(Cam *cam) {
    cam->field_60 = cam_src_a.a;
    cam->field_64 = cam_src_a.b;
    table_801724b0[game_state.field_40](cam);
}
