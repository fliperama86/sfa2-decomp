/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void spawn_effect_object_type_3_at_object(Object *object)
{
  Object *new_object;
  u8 side;
  new_object = (Object *)pop_block_from_pool();
  if (new_object != 0)
  {
    new_object->active_count = 1;
    new_object->field_02 = 0x37;
    new_object->table_index = 3;
    new_object->field_1c = object->field_1c;
    new_object->field_0e = object->field_0e;
    new_object->scroll_factor = object->scroll_factor;
    new_object->facing_side = object->facing_side;
    new_object->pos_x = object->pos_x;
    new_object->pos_y = object->pos_y;
    side = object->team_id;
    new_object->spawn_x = 0x60;
    new_object->spawn_y = 0x1e0;
    new_object->parent = object;
    new_object->field_90 = (void *) 0x800fb100;
    new_object->data_ptr_a = spawned_object_data_a;
    new_object->data_ptr_b = spawned_object_data_b;
    new_object->team_id = side;
  }
}
