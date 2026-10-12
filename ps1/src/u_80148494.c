/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_80148494(Object *object, int a_arg, int b_arg)
{
  u16 a = a_arg;
  u16 b = b_arg;
  u8 i;
  Object *new_object;
  u16 side;
  Object *new_var;
  for (i = 0; i <= (object->direction_index >> 1); i++)
  {
    new_object = (Object *)func_8011f1e0();
    if (new_object != 0)
    {
      new_object->active_count = 1;
      new_object->field_02 = 7;
      new_object->table_index = object->direction_index >> 1;
      new_object->field_0c = 0;
      new_object->image_row = 0;
      new_object->active_state = 0xff;
      new_object->variant_index = i;
      new_var = new_object;
      new_var->field_0e = object->field_0e;
      side = object->field_1c;
      new_var->parent = object;
      new_var->spawn_x = 0x60;
      new_var->spawn_y = 0x1e0;
      new_var->data_ptr_a = data_80172a48;
      new_var->health = a;
      new_var->y_offset = b;
      new_var->field_90 = (void *) 0x800fb100;
      new_var->data_ptr_b = data_80173c9c;
      new_var->field_1c = side;
    }
  }

}
