/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void spawn_effect_objects_at_offset(Object *object, int x, int y)
{
  int dx;
  Object *new_var;
  Object *o;
  dx = x;
  o = (Object *)pop_block_from_pool();
  if (o != 0)
  {
    o->active_count = 1;
    o->field_02 = 0x37;
    o->table_index = 2;
    o->field_1c = object->field_1c;
    o->field_0e = object->field_0e;
    o->scroll_factor = object->scroll_factor;
    o->team_id = object->team_id;
    o->parent = object;
    o->facing_side = object->facing_side;
    o->spawn_x = 0x60;
    o->spawn_y = 0x1e0;
    o->data_ptr_a = &spawned_object_data_a;
    o->field_90 = (void *) 0x800fb100;
    o->data_ptr_b = &spawned_object_data_b;
    if (o->facing_side != 0)
    {
      dx = -x;
    }
    o->pos_x = dx + object->pos_x;
    o->pos_y = object->pos_y - y;
    if (object->active_state == 0)
    {
      x = 0;
      do
      {
        o = (Object *)pop_block_from_pool();
        if (o != 0)
        {
          o->active_count = 1;
          o->field_02 = 0x37;
          o->table_index = x;
          o->field_1c = object->field_1c;
          o->field_0e = object->field_0e;
          o->scroll_factor = object->scroll_factor;
          o->facing_side = object->facing_side;
          o->pos_x = object->pos_x;
          o->pos_y = object->ground_y;
          o->team_id = object->team_id;
          new_var = (o->parent = object);
          o->spawn_x = 0x60;
          o->spawn_y = 0x1e0;
          o->data_ptr_a = &spawned_object_data_a;
          o->field_90 = (void *) 0x800fb100;
          o->data_ptr_b = &spawned_object_data_b;
        }
        x++;
      }
      while (((u16) x) < 2);
    }
  }
}
