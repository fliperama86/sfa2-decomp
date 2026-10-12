/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_80026ec8_slot27[];
extern u16 data_80026aac_slot27[];

void set_sequence_and_position_by_other_kind_slot27(Object *obj) {
    int *sel = data_8019045c;

    *sel = game_state.other_object->kind;
    set_sequence_from_table_5(obj, (s16)*sel, data_80026ec8_slot27[game_state.other_object->side]);
    *sel = *sel * 2;
    obj->pos_x = data_80026aac_slot27[data_8019045c[0]];
    obj->pos_y = data_80026aac_slot27[data_8019045c[0] + 1];
}
