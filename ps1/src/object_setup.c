/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void select_box_tables(Object *object) {
    if (object->side == 0) {
        if (object->field_cd) object->box_tables = box_tables_left_alt;
        else object->box_tables = box_tables_left;
    } else {
        if (object->field_cd) object->box_tables = box_tables_right_alt;
        else object->box_tables = box_tables_right;
    }
    object->boxes_a = object->box_tables->boxes_a;
    object->boxes_b = object->box_tables->boxes_b;
    object->boxes_c = object->box_tables->boxes_c;
    object->wide_boxes = object->box_tables->wide_boxes;
    object->unknown_148 = object->box_tables->unknown_10;
}

void init_table_fields(Object *object) {
    if (object->field_d8) {
        object->field_19e = object->field_19c;
        if (game_state.mode != 3) object->field_19e = 6;
    }
    object->field_65 = side_table_65[object->side];
    object->field_66 = side_table_66[object->side];
    object->field_162 = kind_table_162[object->kind];
    object->field_154 = kind_table_154[object->kind];
}

void set_start_position(Object *object) {
    object->pos_x = 0x280;
    object->pos_y = 0xd0;
    object->field_70 = 0xd0;
}

void set_side_field_0d(Object *object) {
    object->field_0d = side_table_0d[object->side];
}
