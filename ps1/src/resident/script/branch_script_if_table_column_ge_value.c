/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void branch_script_if_table_column_ge_value(Object *object)
{
  short new_var2;
  u16 v = *(script_cursor++);
  u16 new_var;
  new_var2 = v;
  new_var = new_var2 >> 8;
  object->script_byte_hi = new_var;
  object->script_byte_lo = v;
  if ((*object).table_column >= (new_var2 & 0x1f))
  {
    read_script_word_and_dispatch(object);
  }
  else
  {
    skip_script_block_then_read_opcode(object);
  }
}
