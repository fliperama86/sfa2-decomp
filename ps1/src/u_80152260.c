/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void init_object_spawn_and_set_sequence(Object *object)
{
  PtrTbl39 t;
  t = data_8016da38;
  object->spawn_x = 0x60;
  object->spawn_y = 0x1e0;
  object->state_0++;
  object->field_0c = 0;
  object->field_0f = 1;
  object->image_row = 0;
  object->spawn_mode = 4;
  object->pos_y = 0xf8 - object->pos_y;
  if (object->facing_side == 0)
  {
    object->field_0a = 1;
  }
  set_sequence_entry(object, t.entries[object->table_index][data_8018024c[object->table_index]]);
}
