/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 fourth_layer_offset_x;

void update_layer_follow_and_flag_blink_slot06_02(Slot06Layer *layer);

void update_layer_follow_and_flag_blink_slot06_02(Slot06Layer *layer) {
    Slot06Layer *l2 = (Slot06Layer *)main_layer;
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
    d += fourth_layer_offset_x;
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
