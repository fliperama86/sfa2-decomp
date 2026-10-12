/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80190542;

void scroll_layer_left_and_update_offsets_slot06_04(Slot06Layer *layer);
void set_layer_flags_and_timer_from_table_slot06_04(Slot06Layer *layer);

void scroll_layer_left_and_update_offsets_slot06_04(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x += -0x18000;
        layer->countdown_timer -= 1;
        if (layer->countdown_timer == 0) {
            layer->table_index = (layer->table_index + 2) & 0xe;
            set_layer_flags_and_timer_from_table_slot06_04(layer);
        }
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d += data_80190542;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d += layer->field_0e;
    d += layer->pos_y_int;
    layer->field_26 = d;
}
