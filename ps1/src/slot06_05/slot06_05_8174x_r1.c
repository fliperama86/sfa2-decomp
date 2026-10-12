/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_layer_follow_main_layer_eighth_slot06_05(Slot06Layer *layer);

void update_layer_follow_main_layer_eighth_slot06_05(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x -= 0x1800;
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d -= d / 8;
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
