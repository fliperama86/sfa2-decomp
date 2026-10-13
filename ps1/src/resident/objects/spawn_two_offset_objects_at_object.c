/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void spawn_two_offset_objects_at_object(Object *object)
{
  u16 i;
  Object *new_object;
  int offset;
  for (i = 0; i < 2; i++)
  {
    new_object = (Object *)pop_block_from_pool();
    if (new_object != 0)
    {
      new_object->active_count = 1;
      new_object->field_02 = 8;
      new_object->field_0c = 0;
      new_object->image_row = 0;
      new_object->field_0e = object->field_0e;
      new_object->facing_side = i;
      new_object->pos_x = object->pos_x;
      new_object->pos_y = object->pos_y;
      offset = table_8017cd40[i];
      new_object->spawn_x = 0x60;
      new_object->spawn_y = 0x1e0;
      new_object->data_ptr_a = spawned_object_data_a;
      new_object->data_ptr_b = spawned_object_data_b;
      new_object->field_90 = (void *) 0x800fb100;
      new_object->parent = object;
      new_object->pos_x += offset;
      new_object->pos_y = new_object->pos_y;
    }
  }

}
