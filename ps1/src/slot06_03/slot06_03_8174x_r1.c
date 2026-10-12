/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_layer_motion_and_follow_scroll_slot06_03(Slot06Layer *layer);
void init_layer_motion_and_advance_state_slot06_03(Slot06Layer *layer);
void move_layer_until_above_screen_slot06_03(Slot06Layer *layer);
void move_layer_with_x_y_velocity_updates_slot06_03(Slot06Layer *layer);

void update_layer_motion_and_follow_scroll_slot06_03(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (layer->state_index == 0) {
        init_layer_motion_and_advance_state_slot06_03(layer);
    } else if (layer->state_index == 1) {
        move_layer_until_above_screen_slot06_03(layer);
    } else if (layer->state_index == 2) {
        move_layer_with_x_y_velocity_updates_slot06_03(layer);
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d += layer->field_0e;
    d += layer->pos_y_int;
    layer->field_26 = d;
}
