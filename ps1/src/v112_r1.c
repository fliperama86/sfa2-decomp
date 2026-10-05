/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Same form as func_8013606c, which does this for the first camera object:
   the members are separate externs here. With members of one struct the
   compiler schedules the pointer chain before the two halfword loads. */

void func_801368fc(void) {
    u16 p, q;
    table_80172460[game_state.field_40](&cam_obj);
    p = cam_f22;
    q = cam_f26;
    cam_f12 = p;
    cam_f16 = q;
    cam_f50 = cam_f54;
    cam_f50 += cam_f48 * 2;
    cam_f50 += cam_f4a * 2;
    cam_f2a = p;
    cam_f2e = q;
}
