/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_score_value_from_target_field(Actor *a) {
    Object *o = a->target;
    a->score_value = a->table_score = table_80181380[a->number_value];
    a->score_value = o->field_170;
}

void update_owner_field_text(Actor *a) {
    table_80180384[a->side]->buf[6] = table_80180348[a->owner->effect_code_bc >> 1];
    add_item_to_table(table_80180384[a->side]);
}

void call_function_at_data_pointer(void) {
    data_80180ed8();
}
