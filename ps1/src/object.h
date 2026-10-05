/* Reconstruction. Names/roles inferred, not original symbols. */
#ifndef OBJECT_H
#define OBJECT_H

#include "types.gen.h"

extern Object player_left, player_right;

/* metrics.c */
void build_metrics(Object *object);
/* object_setup.c */
void select_box_tables(Object *object);
void init_table_fields(Object *object);
void set_start_position(Object *object);
void set_side_field_0d(Object *object);

#endif
