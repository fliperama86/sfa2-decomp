/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void init_cam_position_with_fixed_origin(Cam *cam);

void update_layer_follow_main_layer_quarter_slot06_05(Slot06Layer *o) {
    Slot06Layer *l2;
    s16 d;

    if (o->mode == 0) {
        init_cam_position_with_fixed_origin((Cam *)o);
    } else if (o->mode == 1) {
        l2 = (Slot06Layer *)main_layer;
        d = l2->field_22;
        d -= l2->ref_x;
        d -= d / 4;
        d += o->ref_x;
        d += o->pos_x_int;
        o->field_22 = d;
        d = l2->field_26;
        d -= l2->field_0e;
        d -= d / 4;
        d += o->field_0e;
        d += o->pos_y_int;
        o->field_26 = d;
    }
}
