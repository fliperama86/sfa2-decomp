/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Aliases of player_left.field_b2 (read as u8) and player_right.field_b2 (read as s16); the member width differs. */

/* Form found by automatic permutation search. */
Tri *func_801250c0(Object *object, int offset)
{
  Tri *new_var;
  offset = (table_801aa4d8[object->field_02] - offset) & 0x1f;
  new_var = table_801ac318[object->field_02];
  return new_var + offset;
}
