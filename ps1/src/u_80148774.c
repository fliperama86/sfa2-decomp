/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_80148774(Object *object)
{
  u16 i;
  Object *new_object;
  int offset;
  for (i = 0; i < 2; i++)
  {
    new_object = (Object *)func_8011f1e0();
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
      new_object->data_ptr_a = data_80172a48;
      new_object->data_ptr_b = data_80173c9c;
      new_object->field_90 = (void *) 0x800fb100;
      new_object->parent = object;
      new_object->pos_x += offset;
      new_object->pos_y = new_object->pos_y;
    }
  }

}
