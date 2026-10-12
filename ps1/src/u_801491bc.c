/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by automatic permutation search. */
void release_object_and_store_player_image(Object *object)
{
  Object *other = object->parent;
  int index;
  int w;
  int d;
  Object *new_var;
  reset_and_push_object_to_pool_table(object);
  d = other->field_02;
  new_var = other;
  if (d == 0)
  {
    index = 0x10;
    d = player_left.field_d4;
    w = 0x100;
  }
  else
  {
    index = 0xb;
    d = player_right.field_d4;
    w = 0x120;
  }
  store_image_rows_to_table(new_var, index, w, (5 * d) + 0x1e0);
  queue_image_upload_job(0, 6);
}
