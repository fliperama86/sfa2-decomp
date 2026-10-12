/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80136b34(Cam *cam) {
    cam->field_04++;
    cam->field_70 = 0;
    load_cam_sources_and_call_kind_handler(cam);
}

void load_cam_sources_and_call_kind_handler(Cam *cam) {
    cam->src_a = cam_src_a.a;
    cam->src_b = cam_src_a.b;
    table_801724b0[game_state.kind_index](cam);
}
