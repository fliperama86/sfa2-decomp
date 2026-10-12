/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: 5 slots. The original stores the constant 6 (in $v1) before the two loads of
   script_arg; the build hoists the loads above the stores and uses $v0. */


/* Form found by automatic permutation search. */
void set_states_if_enemy_near_random_bit(Object *object)
{
  u32 *table;
  if (check_enemy_unit_or_other_in_range(object))
  {
    object->field_208 = 6;
    object->state_0 = 1;
    object->state_2 = 6;
    object->field_209 = 0;
    object->state_1 = 0;
    object->state_3 = 4;
    object->field_221 = object->script_arg;
    if (object->script_arg != 0)
    {
      if ((object->list_offset != 0) && (object->table_flag != 0))
      {
        if ((player_left.kind == 0xd) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[21];
        }
        else
          if ((player_left.kind == 0x10) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[22];
        }
        else
          if ((player_left.kind == 0x11) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[23];
        }
        else
          if ((player_left.kind == 0x13) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[24];
        }
        else
          if (object->side == 0)
        {
          table = table_8017d4d0[object->kind];
        }
        else
        {
          table = table_8017d534[object->kind];
        }
        if (test_random_bit_in_table(object, table) != 0)
        {
          return;
        }
      }
      object->field_221 = 0;
    }
  }
  else
  {
    read_script_word_and_dispatch(object);
  }
}
