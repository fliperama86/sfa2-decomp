/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* box_margin is declared as an array (externs.h) so that the load is treated as a member access and orders after the state_2 store. */

void init_object_position_and_sequence_from_table(Object *object)
{
  u16 margin;
  u8 t = object->state_2;
  object->state_2 = t + 1;
  margin = box_margin[0];
  object->pos_y = 0x70;
  object->field_01 = 2;
  object->data_ptr_a = spawned_object_data_a;
  object->data_ptr_b = spawned_object_data_b;
  object->spawn_x = 0x60;
  object->field_90 = (void *) 0x800fb100;
  object->pos_x = margin + 0xc0;
  set_sequence_from_table_5(object, 0x41, table_8017c7f8);
  advance_state_2_on_sequence_end_set_depth(object);
}
