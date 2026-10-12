/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_80190542;

void update_layer_follow_and_flag_blink_slot06_02(Slot06Layer *layer);

void update_layer_follow_and_flag_blink_slot06_02(Slot06Layer *layer) {
    Slot06Layer *l2 = (Slot06Layer *)data_801aa5d4;
    s16 d;

    layer->field_8b--;
    d = 0;
    if (layer->field_8b == 0) {
        layer->field_8b = 3;
        d = 0x400;
    }
    layer->flags_bits = d;
    d = l2->field_22;
    d -= l2->ref_x;
    d += data_80190542;
    d -= d / 16;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d -= d / 8;
    d += layer->field_0e;
    d += layer->pos_y_int;
    layer->field_26 = d;
}
